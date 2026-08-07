#include <WiFi.h>
#include <HTTPClient.h>
#include <DHTesp.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

#define DHT_PIN 15

DHTesp dhtSensor;

// ===============================
// SUPABASE
// ===============================
String supabaseUrl = "https://ykpkgdjdpmtmpfyapybx.supabase.co/rest/v1/mediciones";

String supabaseApiKey =
"sb_publishable_JvGus1A4EbpPvnTBU2vuDw_pKXS4lXi";

// ===============================
// SENSORES SIMULADOS
// ===============================
String sensores[] = { "sensor_001", "sensor_002", "sensor_003" };
int cantidadSensores = 3;
int indiceSensorActual = 0;

void enviarMedicion(String idDispositivo, String tipoVariable, float valor, String unidad) {
  HTTPClient http;
  http.begin(supabaseUrl);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("apikey", supabaseApiKey);
  http.addHeader("Authorization", "Bearer " + supabaseApiKey);
  http.addHeader("Prefer", "return=minimal");

  String json = "{";
  json += "\"id_dispositivo\":\"" + idDispositivo + "\",";
  json += "\"tipo_variable\":\"" + tipoVariable + "\",";
  json += "\"valor\":" + String(valor, 2) + ",";
  json += "\"unidad\":\"" + unidad + "\"";
  json += "}";

  int httpCode = http.POST(json);
  Serial.print("[" + idDispositivo + " | " + tipoVariable + "] HTTP Code: ");
  Serial.println(httpCode);
  Serial.println(http.getString());

  http.end();
}

void setup() {
  Serial.begin(115200);
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);

  WiFi.begin(ssid, password);
  Serial.print("Conectando WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado");
}

void loop() {
  TempAndHumidity data = dhtSensor.getTempAndHumidity();

  if (isnan(data.temperature) || isnan(data.humidity)) {
    Serial.println("Error leyendo DHT22");
    delay(2000);
    return;
  }

  String idDispositivo = sensores[indiceSensorActual];

  float temp = data.temperature;
  float humedad = data.humidity;
  float presion = random(10080, 10250) / 10.0;

  Serial.println("================");
  Serial.println("Dispositivo: " + idDispositivo);
  Serial.println("Temp: " + String(temp) + "  Hum: " + String(humedad) + "  Pres: " + String(presion));

  if (WiFi.status() == WL_CONNECTED) {
    enviarMedicion(idDispositivo, "temperature", temp, "Celsius");
    delay(1000);
    enviarMedicion(idDispositivo, "humidity", humedad, "%");
    delay(1000);
    enviarMedicion(idDispositivo, "pressure", presion, "hPa");
  }

  // Rotar al siguiente sensor para el próximo ciclo
  indiceSensorActual = (indiceSensorActual + 1) % cantidadSensores;

  delay(30000);
}

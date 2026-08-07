# Backend IoT Histórico con Supabase y Wokwi

Backend de una aplicación IoT orientada al almacenamiento histórico de variables
climáticas (temperatura, humedad, presión atmosférica), usando un circuito
ESP32 + DHT22 simulado en Wokwi que envía datos por HTTP hacia Supabase.

## Arquitectura

```
ESP32 (Wokwi) --Wokwi-GUEST--> Internet --HTTP POST--> Supabase (REST API) --> PostgreSQL
```

1. El ESP32 simulado en Wokwi se conecta a internet usando la red `Wokwi-GUEST`.
2. Lee temperatura y humedad del DHT22 (la presión se simula por software).
3. Envía cada variable como una fila independiente a la tabla `mediciones`
   de Supabase, vía la API REST autogenerada (PostgREST), usando `HTTP POST`.
4. Los datos quedan almacenados como histórico, listos para ser consultados
   por un futuro dashboard.

## Estructura del repositorio

```
backend-iot-historico/
├── README.md
├── .gitignore
├── wokwi/
│   ├── sketch.ino          # Código del ESP32
│   ├── libraries.txt       # Librerías usadas
│   └── diagram.json        # Circuito (exportar desde Wokwi, ver DIAGRAM_README.md)
├── database/
│   └── schema.sql          # Esquema completo de Supabase (tablas + RLS)
└── docs/
    ├── captura-circuito.png
    ├── captura-serial-monitor.png
    ├── captura-tabla-mediciones.png
    └── captura-estructura-github.png
```

## Modelo de datos

**Tabla `dispositivos`**

| Campo | Tipo |
|---|---|
| id_dispositivo | text (PK) |
| ubicacion | text |
| descripcion | text |
| creado_en | timestamptz |

**Tabla `mediciones`** (histórico — una fila por variable)

| Campo | Tipo |
|---|---|
| id | uuid (PK) |
| id_dispositivo | text (FK → dispositivos) |
| tipo_variable | text |
| valor | numeric |
| unidad | text |
| timestamp | timestamptz |

Ver el script completo en [`database/schema.sql`](database/schema.sql).

## Dispositivos simulados

El sketch rota entre 3 identificadores de dispositivo en cada ciclo, para
demostrar el registro histórico multi-dispositivo:

- `sensor_001`
- `sensor_002`
- `sensor_003`

## Cómo correrlo

1. Abrir el proyecto en [Wokwi](https://wokwi.com).
2. Cargar `wokwi/sketch.ino` y `wokwi/diagram.json`.
3. En Supabase, ejecutar `database/schema.sql` en el **SQL Editor**.
4. En `sketch.ino`, colocar la URL del proyecto y la `anon` / `publishable` key
   (Project Settings → API en el dashboard de Supabase).
5. Dar **Play** en Wokwi. El Serial Monitor debe mostrar `HTTP Code: 201`
   por cada variable enviada.
6. Verificar en **Supabase → Table Editor → mediciones** que las filas
   aparecen correctamente.

## Resultado esperado

Backend funcional, disponible para ser consumido por un frontend tipo
dashboard climático, donde se puedan visualizar mediciones actuales e
históricas y generar gráficos de tendencias por variable y por dispositivo.

## Autor

_Nombre: ____________________________
_Materia: ____________________________

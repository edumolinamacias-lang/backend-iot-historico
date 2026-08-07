-- ============================================
-- Backend IoT Histórico - Esquema Supabase
-- ============================================

-- 1. Tabla de dispositivos
create table if not exists dispositivos (
  id_dispositivo text primary key,
  ubicacion text,
  descripcion text,
  creado_en timestamptz default now()
);

-- 2. Tabla histórica de mediciones (una fila por variable)
create table if not exists mediciones (
  id uuid primary key default gen_random_uuid(),
  id_dispositivo text references dispositivos(id_dispositivo),
  tipo_variable text not null,      -- 'temperature', 'humidity', 'pressure'
  valor numeric not null,
  unidad text not null,             -- 'Celsius', '%', 'hPa'
  timestamp timestamptz default now()
);

-- 3. Registrar los dispositivos simulados
insert into dispositivos (id_dispositivo, ubicacion, descripcion)
values
  ('sensor_001', 'Quevedo, Ecuador', 'ESP32 + DHT22 simulado en Wokwi #1'),
  ('sensor_002', 'Quevedo, Ecuador', 'ESP32 + DHT22 simulado en Wokwi #2'),
  ('sensor_003', 'Quevedo, Ecuador', 'ESP32 + DHT22 simulado en Wokwi #3')
on conflict (id_dispositivo) do nothing;

-- 4. Seguridad (Row Level Security)
alter table mediciones enable row level security;

create policy "Permitir insert publico"
on mediciones for insert
to anon
with check (true);

create policy "Permitir lectura publica"
on mediciones for select
to anon
using (true);

alter table dispositivos enable row level security;

create policy "Permitir lectura publica dispositivos"
on dispositivos for select
to anon
using (true);

-- ============================================
-- Consultas de verificación
-- ============================================

-- Ver estructura de mediciones
-- select column_name, data_type, is_nullable
-- from information_schema.columns
-- where table_name = 'mediciones'
-- order by ordinal_position;

-- Ver últimas mediciones por dispositivo
-- select id_dispositivo, tipo_variable, valor, unidad, timestamp
-- from mediciones
-- order by timestamp desc
-- limit 20;

-- ============================================
-- Realtime (para el dashboard interactivo)
-- ============================================
alter publication supabase_realtime add table mediciones;

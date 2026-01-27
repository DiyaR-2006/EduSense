# EduSmart Classroom Controller (ESP32-S3)

An IoT-enabled smart classroom system designed for energy efficiency and safety.

## 🌟 Key Features
- **Climate Control**: Auto-activates Fan/AC based on DHT22 thresholds.
- **Occupancy Lighting**: PIR-based energy saving.
- **Safety**: Real-time MQ-2 smoke monitoring with audible alarms.
- **Cloud Integration**: Blynk IoT Dashboard for remote telemetry and manual control.

## 🔧 Hardware Setup
- **Microcontroller**: ESP32-S3
- **Sensors**: DHT22, PIR, MQ-2 (Analog)
- **Actuators**: Relay, LED, Active Buzzer
- **Display**: 16x2 I2C LCD (Offline version)

## 📌 Wiring (ESP32-S3)
- DHT22: GPIO 18
- PIR: GPIO 17
- Smoke: GPIO 4
- Relay: GPIO 12
- LED: GPIO 15
- Buzzer: GPIO 13

## ☁️ Blynk Datastreams
- V0: Temperature
- V1: Humidity
- V2: CO2/Smoke
- V5: Occupancy
- V7: Light Switch
- V8: Fan Switch

## 📂 Libraries Required
- `Blynk`
- `DHT sensor library`
- `LiquidCrystal I2C`

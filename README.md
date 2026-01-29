## EduSense
Smart Classroom Ecosystem (ESP32-S3)
An IoT-enabled smart classroom system designed for energy efficiency and safety.
EduSense is an IoT-driven solution designed to optimize the learning environment by monitoring AQI and smoke alert, temperature, and lighting while automating energy consumption. This project leverages the ESP32 microcontroller and Blynk IoT cloud to create a sustainable and responsive classroom.

## Key Features
- **Climate Control**: Auto-activates Fan based on DHT22 thresholds.
- **Occupancy Lighting**: PIR-based energy saving.
- **Safety**: Real-time MQ-2 smoke monitoring with audible alarms used as proxy for simulation.
- **Cloud Integration**: Blynk IoT Dashboard for remote telemetry and manual control.

  
## Hardware Setup
- **Microcontroller**: ESP32-S3-wroom
- **Sensors**: DHT22, PIR, MQ-2 (Analog)
- **Actuators**: Relay, LED, Active Buzzer
- **Display**: 16x2 I2C LCD (Offline version)

## Software
- C++ (Arduino Framework)
- Blynk IoT Cloud (Online version)
- Wokwi (Simulation)

## Wiring (ESP32-S3)
- DHT22: GPIO 18
- PIR: GPIO 7
- Smoke: GPIO 4
- Relay: GPIO 12
- LED: GPIO 15
- Buzzer: GPIO 13

## Blynk Datastreams
- V0: Temperature
- V1: Humidity
- V2: AQI/Smoke
- V5: Occupancy
- V7: Light Switch
- V8: Fan Switch

## Libraries Required
- `Blynk`
- `DHT sensor library`
- `LiquidCrystal I2C`

## Setup & Installation
- Blynk Setup:
  --Create a new Template in Blynk.
  --Define Datastreams (V0-V12) for sensors and actuators.
  --Copy your Template ID and Auth Token.
- Wokwi Simulation:
- Open the diagram.json and sketch.ino in Wokwi.
- Paste your Blynk credentials at the top of the sketch. Ensure libraries.txt includes: Blynk, DHT sensor library.
- Run: Click the "Play" button in Wokwi and monitor the data on your Blynk mobile app.

## Future Scope
- Integration of AI for predictive energy usage analytics.
- Centralized dashboard for school-wide facility management (B2B)

  
## 📁 Repository StructurePlaintext

├──Codezen 2026 ppt.pdf    #TEMPLATE FOR PPT

├── LICENSE        

├── README.md            # Project documentation

├── libraries.txt        # Wokwi library dependencies

├── OFFLINE-sketch.ino           # Main application logic when system not connected with wifi 

├── ONLINE-sketch.ino           # Main application logic when system connected with wifi 

├── OFFLINE-diagram.json         # Wokwi circuit configurations

├── ONLINE-diagram.json         # Wokwi circuit configurations

└── Sync & Sense.pdf     # 6-slide hackathon presentation




## **END OF DOCUMENTATION**

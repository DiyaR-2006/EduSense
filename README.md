# EduSense
Smart Classroom Ecosystem 
EduSense: Smart Classroom EcosystemEduSense is an IoT-driven solution designed to optimize the learning environment by monitoring air quality, temperature, and lighting while automating energy consumption. This project leverages the ESP32 microcontroller and Blynk IoT cloud to create a sustainable, responsive classroom.

🚀 Key FeaturesEnvironmental 
- Monitoring: Real-time tracking of Temperature, Humidity, and $CO_2$ levels using DHT22 and MQ-2 sensors.
- Energy Efficiency: PIR-based occupancy sensing to automatically toggle lights and fans, reducing unnecessary power waste.
- Safety Alerts: Instant mobile notifications via Blynk when smoke or fire is detected.
- Automation: Servo-controlled "Presentation Mode" to dim lights and lower projector screens with a single tap.

🛠️ Tech Stack
- Hardware: ESP32, DHT22, HC-SR501 (PIR), MQ-2 (Smoke), SG90 Servo, 5V Relay.
- Software: C++ (Arduino Framework), Blynk IoT Cloud.
- Simulation: Wokwi.

  📁 Repository StructurePlaintext

├── sketch.ino        # Main application logic

├── libraries.txt     # Wokwi library dependencies

├── diagram.json      # Wokwi circuit configuration

├── README.md         # Project documentation

└── EduSense_PPT.pdf  # 6-slide hackathon presentation

⚙️ Setup & Installation
- Blynk Setup:Create a new Template in Blynk.Define Datastreams (V0-V12) for sensors and actuators.
- Copy your Template ID and Auth Token.
- Wokwi Simulation:Open the diagram.json and sketch.ino in Wokwi.
- Paste your Blynk credentials at the top of the sketch.Ensure libraries.txt includes: Blynk, DHT sensor library, and ESP32Servo.
- Run: Click the "Play" button in Wokwi and monitor the data on your Blynk mobile app.

📈 Future Scope
- Integration of AI for predictive energy usage analytics.
- Centralized dashboard for school-wide facility management (B2B)

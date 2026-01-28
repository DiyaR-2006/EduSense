/**
 * @file Offline-EduSense.ino
 * @brief An automated environmental monitoring and safety system using ESP32-S3-WROOM.
 * @version 1.0

 * * Features:
 * - Occupancy-based lighting control (PIR Sensor + LED)
 * - Automated Temperature and Humidity Control (DHT22 + Relay)
 * - Gas/Smoke Safety Alert (MQ-2 Sensor + Buzzer)
 * - Real-time Telemetry (I2C LCD)
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

/* ----------------- HARDWARE MAPPING ----------------- */
// I2C Interface
#define I2C_SDA      8    
#define I2C_SCL      9    

// Digital & Analog Sensors
#define DHT_PIN      18   
#define PIR_PIN      7    
#define SMOKE_PIN    4    

// Actuators (Outputs)
#define RELAY_PIN    12   // Controls high-voltage fans/AC
#define BUZZER_PIN   13   // Safety alarm
#define LED_PIN      15   // Smart lighting

/* ----------------- SYSTEM THRESHOLDS ----------------- */
#define DHT_TYPE     DHT22
#define SMOKE_LIMIT  1000  // Sensitivity threshold for gas/smoke
#define TEMP_LIMIT   25    // Target max temperature (°C)
#define HUM_LIMIT    50    // Target max humidity (%)

// Component Initialization
LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHT_PIN, DHT_TYPE);

/**
 * @brief Standard Arduino Setup Function
 */
void setup() {
  Serial.begin(115200);
  
  // Initialize Communication Protocols
  Wire.begin(I2C_SDA, I2C_SCL);
  lcd.init();
  lcd.backlight();
  dht.begin();

  // Configure Pin Modes
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Power-On Self Test (POST)
  lcd.print("SYSTEM STARTING");
  delay(2000); 
  lcd.clear();
}

/**
 * @brief Main Execution Loop
 */
void loop() {
  // 1. Data Acquisition
  int smokeValue = analogRead(SMOKE_PIN);
  int motionDetected = digitalRead(PIR_PIN);
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  
  // 2. Occupancy & Lighting Control Logic
  handleLighting(motionDetected);

  // 3. Environmental & Climate Control
  handleClimate(temperature, humidity);

  // 4. Safety Monitor & Visual Feedback
  updateDisplay(temperature, humidity, smokeValue);

  // Global loop timing (DHT22 requires ~2s between reads)
  delay(2000); 
}

/* ----------------- MODULAR FUNCTIONS ----------------- */

/**
 * @brief Manages energy-saving lighting based on PIR detection.
 */
void handleLighting(int motion) {
  if (motion == HIGH) {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("[MOTION] Occupancy detected: Lights ON");
    // Extra delay within logic if specific dwell time is required
  } else {
    digitalWrite(LED_PIN, LOW);
  }
}

/**
 * @brief Logic for triggering relay-based cooling systems.
 */
void handleClimate(float t, float h) {
  if (isnan(t) || isnan(h)) return; // Skip if sensor error

  if (t > TEMP_LIMIT || h > HUM_LIMIT) {
    digitalWrite(RELAY_PIN, HIGH);
    Serial.printf("[CLIMATE] Active | Temp: %.1fC | Hum: %.1f%%\n", t, h);
  } else {
    digitalWrite(RELAY_PIN, LOW);
  }
}

/**
 * @brief Updates LCD and handles safety buzzer logic.
 */
void updateDisplay(float t, float h, int smoke) {
  // Row 0: Telemetry
  lcd.setCursor(0, 0);
  if (isnan(t) || isnan(h)) {
    lcd.print("ERR: DHT SENSOR");
  } else {
    lcd.print("T:"); lcd.print(t, 1);
    lcd.print("C  H:"); lcd.print(h, 0);
    lcd.print("% ");
  }

  // Row 1: Safety Status
  lcd.setCursor(0, 1);
  if (smoke > SMOKE_LIMIT) {
    lcd.print("** SMOKE ALERT **");
    tone(BUZZER_PIN, 1000);
    Serial.println("[DANGER] Smoke threshold exceeded!");
  } else {
    lcd.print("System: Secured ");
    noTone(BUZZER_PIN);
  }
}

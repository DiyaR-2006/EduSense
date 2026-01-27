/**
 * @file EduSmart_Classroom.ino
 * @author Your Name
 * @brief Automated classroom environment control system using ESP32-S3.
 * * Features:
 * - Climate Control (Temp/Hum via DHT22)
 * - Motion-Activated Lighting (PIR Sensor)
 * - Fire/Smoke Detection & Audible Alarm (MQ series)
 * - Real-time Status Monitoring via I2C LCD
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

/* -------- CONFIGURATION & PINOUT -------- */
// I2C Pins for ESP32-S3
#define I2C_SDA      8    
#define I2C_SCL      9    

// Sensor & Actuator Pins
#define DHT_PIN      18   // Digital Input (Safe pin for S3)
#define PIR_PIN      7    // Digital Input
#define SMOKE_PIN    4    // Analog Input
#define RELAY_PIN    12   // Digital Output (Fan/AC)
#define BUZZER_PIN   13   // Digital Output (Alarm)
#define LED_PIN      15   // Digital Output (Lighting)

// Threshold Settings
#define DHT_TYPE     DHT22
#define SMOKE_LIMIT  1000  
#define TEMP_LIMIT   25
#define HUM_LIMIT    50

/* -------- GLOBAL OBJECTS -------- */
LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHT_PIN, DHT_TYPE);

/**
 * @brief Initializes peripherals and performs a self-test.
 */
void setup() {
  Serial.begin(115200);
  
  // Initialize I2C Bus for ESP32-S3
  Wire.begin(I2C_SDA, I2C_SCL);
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  
  // Initialize Sensors
  dht.begin();
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Boot Screen
  lcd.setCursor(0, 0);
  lcd.print("EduSmart System");
  lcd.setCursor(0, 1);
  lcd.print("Status: Booting");
  
  // Stabilization delay for DHT22
  delay(2000); 
  lcd.clear();
  Serial.println(F("--- System Online ---"));
}

void loop() {
  // 1. Data Acquisition
  float temp       = dht.readTemperature();
  float hum        = dht.readHumidity();
  int   smokeValue = analogRead(SMOKE_PIN);
  int   motion     = digitalRead(PIR_PIN);

  // 2. Climate & Occupancy Logic (Relay Control)
  // Consolidating triggers to prevent logic conflicts
  bool needsCooling = (!isnan(temp) && (temp > TEMP_LIMIT || hum > HUM_LIMIT));
  
  if (needsCooling || motion == HIGH) {
    digitalWrite(RELAY_PIN, HIGH);
  } else {
    digitalWrite(RELAY_PIN, LOW);
  }

  // 3. Lighting Control
  digitalWrite(LED_PIN, (motion == HIGH) ? HIGH : LOW);

  // 4. Safety & Alarm Logic
  if (smokeValue > SMOKE_LIMIT) {
    tone(BUZZER_PIN, 1000); // Trigger 1kHz alarm
  } else {
    noTone(BUZZER_PIN);
  }

  // 5. User Interface (LCD & Serial)
  updateDisplay(temp, hum, smokeValue);
  logToSerial(temp, hum, smokeValue, motion);

  // DHT22 sampling rate limit (0.5Hz)
  delay(2000); 
}

/**
 * @brief Updates the 16x2 LCD with current sensor data.
 */
void updateDisplay(float t, float h, int s) {
  lcd.setCursor(0, 0);
  if (isnan(t) || isnan(h)) {
    lcd.print("Sensor Error   ");
  } else {
    lcd.print("T:"); lcd.print(t, 1);
    lcd.print("C  H:"); lcd.print(h, 0);
    lcd.print("% ");
  }

  lcd.setCursor(0, 1);
  if (s > SMOKE_LIMIT) {
    lcd.print("ALERT: SMOKE!   ");
  } else {
    lcd.print("Status: Secure  ");
  }
}

/**
 * @brief Logs system state to Serial Monitor for debugging.
 */
void logToSerial(float t, float h, int s, int m) {
  Serial.print(F("TEMP: ")); Serial.print(t);
  Serial.print(F(" | HUM: ")); Serial.print(h);
  Serial.print(F(" | SMOKE: ")); Serial.print(s);
  Serial.

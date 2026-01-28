/**
 * @file Offline-EduSense.ino
 * @brief An automated environmental monitoring and safety system using ESP32-S3-WROOM.
 * @version 1.0
 * * Features:
 * - Real-time Environmental Telemetry (Temp/Hum/Smoke)
 * - Motion-based Automated Lighting & Occupancy Tracking
 * - Climate Control Automation (Relay-based Fan)
 * - Remote Manual Override via Blynk Dashboard
 */

#define BLYNK_TEMPLATE_ID "TMPL34Wh926kz"
#define BLYNK_TEMPLATE_NAME "smart classroom"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

/* -------- NETWORK CREDENTIALS -------- */
char auth[] = "B8kkS8CkwuwGIvimt13ARy2qsAti2jbP"; 
char ssid[] = "Wokwi-GUEST";     
char pass[] = "";                

/* -------- PINOUT CONFIGURATION -------- */
#define DHT_PIN      18   // DHT22 Data Pin
#define PIR_PIN      7    // Passive Infrared Sensor (Motion)
#define SMOKE_PIN    4    // MQ Gas Sensor (Analog)
#define RELAY_PIN    12   // Fan/Ventilation Relay
#define BUZZER_PIN   13   // Safety Alarm Buzzer
#define LED_PIN      15   // Smart Lighting Output

#define DHT_TYPE     DHT22
#define TEMP_LIMIT   25   // Temperature activation threshold
#define HUM_LIMIT    50   // Humidity activation threshold
#define SMOKE_LIMIT  1000 // Smoke alert threshold

/* -------- OBJECTS -------- */
DHT dht(DHT_PIN, DHT_TYPE);
BlynkTimer timer;          // Manages cloud update intervals
int occupancyCount = 0;    // Tracks room usage

/* -------- BLYNK DASHBOARD HANDLERS -------- */

/**
 * @brief Manual Light Override (V7)
 */
BLYNK_WRITE(V7) {
  int pinValue = param.asInt();
  digitalWrite(LED_PIN, pinValue);
  Serial.printf("Remote Light Toggle: %s\n", pinValue ? "ON" : "OFF");
}

/**
 * @brief Manual Fan Override (V8)
 */
BLYNK_WRITE(V8) {
  int pinValue = param.asInt();
  digitalWrite(RELAY_PIN, pinValue);
  Serial.printf("Remote Fan Toggle: %s\n", pinValue ? "ON" : "OFF");
}

/**
 * @brief Sends sensor data to Blynk Cloud every 2 seconds.
 * Prevents network congestion while maintaining real-time updates.
 */
void pushTelemetry() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  int s = analogRead(SMOKE_PIN);

  if (!isnan(t) && !isnan(h)) {
    Blynk.virtualWrite(V0, t); // Temp -> V0
    Blynk.virtualWrite(V1, h); // Hum -> V1
  }
  Blynk.virtualWrite(V2, s);   // Smoke -> V2
  
  Serial.printf("Telemetry Update - T:%.1f | H:%.0f | S:%d\n", t, h, s);
}

void setup() {
  Serial.begin(115200);
  
  dht.begin();
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Initialize Network Connection
  Blynk.begin(auth, ssid, pass);

  // Define telemetry interval (2000ms)
  timer.setInterval(2000L, pushTelemetry);
  
  Serial.println("System Initialized and Online");
}

void loop() {
  Blynk.run(); // Process Cloud commands
  timer.run(); // Process timed events

  /* 1. CLIMATE AUTOMATION LOGIC */
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  
  // Trigger Ventilation if environmental safety limits are exceeded
  if (!isnan(t) && (t > TEMP_LIMIT || h > HUM_LIMIT)) {
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("System Alert: Fan activated (Threshold Exceeded)");
    delay(5000);  // Prevent relay "chatter" or rapid cycling
  } else {
    digitalWrite(RELAY_PIN, LOW);   
  }

  /* 2. OCCUPANCY & SMART LIGHTING LOGIC */
  int motion = digitalRead(PIR_PIN);
  if (motion == HIGH) {
    Serial.println("Motion Detected - Light ON");
    digitalWrite(LED_PIN, HIGH);
    
    // Increment total visitor count for cloud analytics
    occupancyCount++;
    Blynk.virtualWrite(V5, occupancyCount);
    
    delay(3000);   // Maintain light for visibility
  } else {
    digitalWrite(LED_PIN, LOW);
    Serial.println("No motion - Energy Saving Mode");
    delay(100);
  }

  /* 3. SAFETY & EMERGENCY LOGIC */
  int smokeVal = analogRead(SMOKE_PIN);
  if (smokeVal > SMOKE_LIMIT) {
    tone(BUZZER_PIN, 1000); // Trigger local high-pitched alarm
    Serial.println("CRITICAL: Smoke Detected!");
  } else {
    noTone(BUZZER_PIN);
  }
}

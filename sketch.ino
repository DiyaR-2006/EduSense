#define BLYNK_TEMPLATE_ID "TMPL34Wh926kz"
#define BLYNK_TEMPLATE_NAME "smart classroom"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHTesp.h>

// Pin Definitions
#define DHT_PIN 15
#define PIR_PIN 13
#define SMOKE_PIN 34
#define RELAY_PIN 2
#define BUZZER_PIN 4
#define BTN_RFID 12  

char ssid[] = "Wokwi-GUEST";
char pass[] = "";

DHTesp dht;
BlynkTimer timer;

void sendSensorData() {
  TempAndHumidity data = dht.getTempAndHumidity();
  Blynk.virtualWrite(V0, data.temperature);
  Blynk.virtualWrite(V1, data.humidity);
  
  int smokeLevel = analogRead(SMOKE_PIN);
  Blynk.virtualWrite(V2, smokeLevel);
  
  if(smokeLevel > 700) {
    digitalWrite(BUZZER_PIN, HIGH); // Alarm ON
    Blynk.logEvent("fire_alert", "CRITICAL: Smoke detected!");
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }
}

void checkAutomation() {
  // Motion-based Lighting
  bool occupied = digitalRead(PIR_PIN);
  digitalWrite(RELAY_PIN, occupied ? HIGH : LOW);
  Blynk.virtualWrite(V4, occupied ? "Occupied" : "Empty");

  // Simulated RFID scan
  if (digitalRead(BTN_RFID) == LOW) {
    Blynk.logEvent("attendance", "Student ID: 2026_ECE_01 Verified");
    delay(500); 
  }
}

void setup() {
  dht.setup(DHT_PIN, DHTesp::DHT22);
  pinMode(PIR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BTN_RFID, INPUT_PULLUP);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(2000L, sendSensorData);
  timer.setInterval(500L, checkAutomation);
}

void loop() {
  Blynk.run();
  timer.run();
}

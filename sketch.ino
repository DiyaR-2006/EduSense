/****************
 BASIC SMART CLASSROOM – ESP32-S3
******************/
#define BLYNK_TEMPLATE_ID "TMPL34Wh926kz"
#define BLYNK_TEMPLATE_NAME "smart classroom"

#include <DHT.h>

/* -------- PIN DEFINITIONS -------- */
#define DHT_PIN     6
#define PIR_PIN     7
#define SMOKE_PIN   4     // Analog pin
#define RELAY_PIN   12
#define BUZZER_PIN  13

/* -------- CONSTANTS -------- */
#define DHT_TYPE DHT11
#define TEMP_LIMIT   25
#define SMOKE_LIMIT  300

/* -------- OBJECTS -------- */
DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(115200);

  pinMode(PIR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  Serial.println("Smart Classroom Started");
}

void loop() {

  /* ---------- DHT11 SENSOR ---------- */
  float temp = dht.readTemperature();
  float Hum = dht.readHumidity();

  if (!isnan(temp)) {
    Serial.print("Temperature: ");
    Serial.println(temp);
    Serial.print("Humidity: ");
    Serial.println(Hum);

    if (temp > TEMP_LIMIT) {
      digitalWrite(RELAY_PIN, HIGH);  // Fan ON
    } else {
      digitalWrite(RELAY_PIN, LOW);   // Fan OFF
    }
  }

  /* ---------- PIR SENSOR ---------- */
  int motion = digitalRead(PIR_PIN);

  if (motion == HIGH) {
    Serial.println("Motion Detected - Light ON");
    digitalWrite(RELAY_PIN, HIGH);
    delay(3000);   // Light ON for 3 sec
  }

  /* ---------- SMOKE SENSOR ---------- */
  int smokeValue = analogRead(SMOKE_PIN);
  Serial.print("Smoke Value: ");
  Serial.println(smokeValue);

  if (smokeValue > SMOKE_LIMIT) {
    Serial.println("Smoke Detected!");
    digitalWrite(BUZZER_PIN, HIGH);
    tone(BUZZER_PIN, 100, 1000);
    delay(2000);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }

  delay(1000);   // Slow loop (easy to read output)
}

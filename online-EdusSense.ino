/****************
 BASIC SMART CLASSROOM – ESP32-S3
******************/
#define BLYNK_TEMPLATE_ID "TMPL34Wh926kz"
#define BLYNK_TEMPLATE_NAME "smart classroom"

#include <DHT.h>

/* -------- PIN DEFINITIONS -------- */
#define DHT_PIN     6       // Temperature and Humidity 
#define PIR_PIN     7      // Motion Detection 
#define SMOKE_PIN   4     // Analog pin
#define RELAY_PIN   12    //digital switch 
#define BUZZER_PIN  13    // ALERT 
#define LED 15
 
/* -------- CONSTANTS -------- */
#define DHT_TYPE DHT22
#define TEMP_LIMIT   25
#define HUM_LIMIT 50
#define SMOKE_LIMIT  1000


/* -------- OBJECTS -------- */
DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(115200);

  pinMode(PIR_PIN, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED, OUTPUT);

  Serial.println("Smart Classroom Started");
}

void loop() {

  /* ---------- DHT11 SENSOR ---------- */
  float temp = dht.readTemperature();
  float Hum = dht.readHumidity();

  if (!isnan(temp) && !isnan(Hum)) {
    Serial.print("Temperature: ");
    Serial.println(temp);
    Serial.print("Humidity: ");
    Serial.println(Hum);

    if (temp > TEMP_LIMIT) {
      digitalWrite(RELAY_PIN, HIGH);
      Serial.println("Fan ON");
      delay(5000);  // Fan ON
    } else {
      digitalWrite(RELAY_PIN, LOW);   // Fan OFF
    }
    if (Hum > HUM_LIMIT) {
      digitalWrite(RELAY_PIN, HIGH);
      Serial.println("Fan ON");
      delay(5000);  // Fan ON
    } else {
      digitalWrite(RELAY_PIN, LOW);   // Fan OFF
    }
  }

  /* ---------- PIR SENSOR ---------- */
  int motion = digitalRead(PIR_PIN);

  if (motion == HIGH) {
    Serial.println("Motion Detected - Light ON");
    digitalWrite(LED, HIGH);
    delay(3000);   // Light ON for 3 sec
  } else {
    digitalWrite(LED, LOW);
    Serial.println("no motion detected - Light OFF");
    delay(100);
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

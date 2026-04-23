#define BLYNK_TEMPLATE_ID "YourTemplateID"
#define BLYNK_DEVICE_NAME "PlantDoctor"
#define BLYNK_AUTH_TOKEN "YourAuthToken"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include "HUSKYLENS.h"
#include "HardwareSerial.h"

char ssid[] = "YourWiFiName";
char pass[] = "YourWiFiPassword";

HUSKYLENS huskylens;
HardwareSerial mySerial(2);

#define RXD2 16
#define TXD2 17

#define LED_PIN 2
#define BUZZER_PIN 4

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  mySerial.begin(9600, SERIAL_8N1, RXD2, TXD2);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  while (!huskylens.begin(mySerial)) {
    Serial.println("HuskyLens not connected!");
    delay(1000);
  }

  Serial.println("System Ready!");
}

void loop() {
  Blynk.run();

  if (!huskylens.request()) return;
  if (!huskylens.available()) return;

  HUSKYLENSResult result = huskylens.read();

  if (result.ID == 1) {
    Serial.println("Plant is HEALTHY");
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, LOW);

    Blynk.virtualWrite(V0, "Plant Status: HEALTHY 🌱");
    Blynk.logEvent("plant_status", "Plant is Healthy");
  }

  else if (result.ID == 2) {
    Serial.println("Plant is DAMAGED");
    digitalWrite(LED_PIN, LOW);

    digitalWrite(BUZZER_PIN, HIGH);
    delay(500);
    digitalWrite(BUZZER_PIN, LOW);

    Blynk.virtualWrite(V0, "Plant Status: DAMAGED ⚠️");
    Blynk.logEvent("plant_status", "Warning! Plant is Damaged!");
  }

  delay(1000);
}

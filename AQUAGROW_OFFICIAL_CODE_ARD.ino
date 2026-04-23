#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

// Pin setup
#define SOIL_PIN A0
#define LDR_PIN A1
#define PUMP_PIN 3
#define LED_PIN 4

int soilValue;
int ldrValue;

void setup() {

  Serial.begin(9600);

  pinMode(PUMP_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  lcd.init();
  lcd.backlight();
}

void loop() {

  soilValue = analogRead(SOIL_PIN);
  ldrValue = analogRead(LDR_PIN);

  // -------- SOIL CONTROL --------
  if(soilValue > 600)   // Soil dry
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Soil: DRY");

    lcd.setCursor(0,1);
    lcd.print("Pump ON");

    digitalWrite(PUMP_PIN,HIGH);
    delay(5000);              // Pump ON for 5 seconds
    digitalWrite(PUMP_PIN,LOW);
  }
  else                  // Soil wet
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Soil: WET");

    lcd.setCursor(0,1);
    lcd.print("Pump OFF");

    digitalWrite(PUMP_PIN,LOW);
  }

  // -------- LDR CONTROL --------
  ldrValue = analogRead(LDR_PIN);

  if(ldrValue > 600)    // Dark
  {
    digitalWrite(LED_PIN,HIGH);
  }
  else                  // Bright
  {
    digitalWrite(LED_PIN,LOW);
  }

  delay(1000);
}

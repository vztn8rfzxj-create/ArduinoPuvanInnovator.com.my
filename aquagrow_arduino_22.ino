#include <Wire.h>
#include <LiquidCrystal_I2C.h>


int  SOIL_PIN=A0;
int LDR_PIN=A1;
int PUMP_PIN=3;
int  LED_PIN=8;


LiquidCrystal_I2C lcd(0x27, 16, 2);


int soilValue = 0;
int ldrValue = 0;

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

  
  if (soilValue < 400) {  
    digitalWrite(PUMP_PIN, HIGH);
    delay(1000); 
    digitalWrite(PUMP_PIN, LOW);
  }

  
  if (ldrValue < 450) { 
    digitalWrite(LED_PIN, HIGH);
  } else {             
    digitalWrite(LED_PIN, LOW);
  }

  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Soil:");
  lcd.print(soilValue);
  lcd.setCursor(0,1);
  lcd.print("Light:");
  lcd.print(ldrValue);

  
  Serial.print("Soil:");
  Serial.print(soilValue);
  Serial.print(" | Light:");
  Serial.println(ldrValue);

  delay(500);
}

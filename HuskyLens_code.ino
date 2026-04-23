#include <SoftwareSerial.h>
#include "HUSKYLENS.h"

#define LED_PIN 13
#define HSK_RX 10
#define HSK_TX 11

HUSKYLENS huskylens;
SoftwareSerial mySerial(HSK_RX, HSK_TX);

void setup(){
  Serial.begin(115200);
  mySerial.begin(9600);
  pinMode(LED_PIN,OUTPUT);
  
 while(!huskylens.begin(mySerial)){
  Serial.println("Waiting for HuskyLens...");
  delay(500);
 }
 Serial.println("HuskyLens detected!");
}

void loop(){
  if (!huskylens.request())return;
  if (!huskylens.available())return;

  HUSKYLENSResult result = huskylens.read();

  if(result.command == COMMAND_RETURN_BLOCK){
    Serial.print("Detected ID:");
    Serial.println(result.ID);

    
  }if(result.ID == 1){
    
  }
  
}

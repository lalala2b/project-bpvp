// Libraries
#include <WiFi.h>
#include <WiFiClientSecure.h>


#define buzzerPin 18
#define magnetSw1 12

void setup(){
  pinMode(buzzerPin,OUTPUT);
  pinMode(magnetSw1,INPUT);

  Serial.begin(9600);
}

void loop(){
  if (digitalRead(magnetSw1)==HIGH){
    Serial.println("open");
    digitalWrite(buzzerPin,HIGH);
  }else{
    digitalWrite(buzzerPin,LOW);
    Serial.println("closed");
  }
  delay(1000);
}
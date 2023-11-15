#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6eCMWr2VA"
#define BLYNK_TEMPLATE_NAME "Test"
#define BLYNK_AUTH_TOKEN "8x_EcDrGAWFZCY3kU6u71xaDRbO8W8Cw"

#include <CTBot.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <DHT.h>
#include <BlynkSimpleEsp32.h>

/*
#define BLYNK_TEMPLATE_ID "TMPL6VP7zHhbO"
#define BLYNK_TEMPLATE_NAME "PROJECT TECH1"
#define BLYNK_AUTH_TOKEN "aLI6cOE9eubSCW2P4hbwyvkRZ9cAg4Wk"
*/
char auth[] = BLYNK_AUTH_TOKEN;
BlynkTimer timer;

#define BOTtoken "6741323250:AAF6Ag62a6mbxs0IsmN9IcCF2NB4FSE6xDw"
#define user_id 5194206843

CTBot bot;

const char* ssid = "lalala";
const char* pass = "00000000";

#define switchRU 13
#define switchK1 12
#define switchKM 14
#define switchHall 27
#define switchK2 26
#define switchK3 25

#define lampuRU 16
#define lampuK1 17
#define lampuKM 5
#define lampuHall 18
#define lampuK2 19
#define lampuK3 21

#define virtualRU V0
#define virtualK1 V1
#define virtualKM V2
#define virtualHall V3
#define virtualK2 V4
#define virtualK3 V5

int stateRU = 0;
int stateK1 = 0;
int stateKM = 0;
int stateHall = 0;
int stateK2 = 0;
int stateK3 = 0;

BLYNK_CONNECTED(){
  Blynk.syncVirtual(virtualRU);
  Blynk.syncVirtual(virtualK1);
  Blynk.syncVirtual(virtualKM);
  Blynk.syncVirtual(virtualHall);
  Blynk.syncVirtual(virtualK2);
  Blynk.syncVirtual(virtualK3);
}

BLYNK_WRITE(virtualRU){
  stateRU = param.asInt();
  digitalWrite(lampuRU,stateRU);
}
BLYNK_WRITE(virtualK1){
  stateK1 = param.asInt();
  digitalWrite(lampuK1,stateK1);
}
BLYNK_WRITE(virtualKM){
  stateKM = param.asInt();
  digitalWrite(lampuKM,stateKM);
}
BLYNK_WRITE(virtualHall){
  stateHall = param.asInt();
  digitalWrite(lampuHall,stateHall);
}
BLYNK_WRITE(virtualK2){
  stateK2 = param.asInt();
  digitalWrite(lampuK2,stateK2);
}
BLYNK_WRITE(virtaulK3){
  stateK3 = param.asInt();
  digitalWrite(lampuK3,stateK3);
}

void setup(){
  Serial.begin(9600);

  bot.wifiConnect(ssid,pass);
  bot.setTelegramToken(BOTtoken);
  Blynk.begin(auth,ssid,pass);

  pinMode(lampuRU,OUTPUT);
  pinMode(lampuK1,OUTPUT);
  pinMode(lampuKM,OUTPUT);
  pinMode(lampuHall,OUTPUT);
  pinMode(lampuK2,OUTPUT);
  pinMode(lampuK3,OUTPUT);

  pinMode(switchRU,INPUT_PULLDOWN);
  pinMode(switchK1,INPUT_PULLDOWN);
  pinMode(switchKM,INPUT_PULLDOWN);
  pinMode(switchHall,INPUT_PULLDOWN);
  pinMode(switchK2,INPUT_PULLDOWN);
  pinMode(switchK3,INPUT_PULLDOWN);

  if (bot.testConnection()){
		Serial.println("\nConnecting success");
    bot.sendMessage(user_id,"Bot siap digunakan. /start");
  }else {
		Serial.println("\nConnecting fail, please reset");
  }
}

void loop(){
  if (digitalRead(switchRU)==HIGH){
    stateRU = !stateRU;
    digitalWrite(lampuRU,stateRU);
    Blynk.virtualWrite(virtualRU,stateRU);
  }else if (digitalRead(switchK1)==HIGH){
    stateK1 = !stateK1;
    digitalWrite(lampuK1,stateK1);
    Blynk.virtualWrite(virtualK1,stateK1);
  }else if (digitalRead(switchKM)==HIGH){
    stateKM = !stateKM;
    digitalWrite(lampuKM,stateKM);
    Blynk.virtualWrite(virtualKM,stateKM);
  }else if (digitalRead(switchHall)==HIGH){
    stateHall = !stateHall;
    digitalWrite(lampuHall,stateHall);
    Blynk.virtualWrite(virtualHall,stateHall);
  }else if (digitalRead(switchK2)==HIGH){
    stateK2 = !stateK2;
    digitalWrite(lampuK2,stateK2);
    Blynk.virtualWrite(virtualK2,stateK2);
  }else if (digitalRead(switchK3)==HIGH){
    stateK3 = !stateK3;
    digitalWrite(lampuK3,stateK3);
    Blynk.virtualWrite(virtualK3,stateK3);
  }

  TBMessage msg;

  if (CTBotMessageText == bot.getNewMessage(msg)){
    if (msg.sender.id == user_id){
      if (msg.text.equals("/lampu_utama")){
        stateRU = !stateRU;
        digitalWrite(lampuRU,stateRU);
        Blynk.virtualWrite(virtualRU,stateRU);
        bot.sendMessage(user_id,"success");

      }else if (msg.text.equals("/lampu_kamar1")){
        stateK1 = !stateK1;
        digitalWrite(lampuK1,stateK1);
        Blynk.virtualWrite(virtualK1,stateK1);
        bot.sendMessage(user_id,"success1");
      }else if (msg.text.equals("/lampu_kmandi")){
        stateKM = !stateKM;
        digitalWrite(lampuKM,stateKM);
        Blynk.virtualWrite(virtualKM,stateKM);
        bot.sendMessage(user_id,"success2");
      }else if (msg.text.equals("/lampu_hall")){
        stateHall = !stateHall;
        digitalWrite(lampuHall,stateHall);
        Blynk.virtualWrite(virtualHall,stateHall);
        bot.sendMessage(user_id,"success3");
      }else if (msg.text.equals("/lampu_kamar2")){
        stateK2 = !stateK2;
        digitalWrite(lampuK2,stateK2);
        Blynk.virtualWrite(virtualK2,stateK2);
        bot.sendMessage(user_id,"success4");
      }else if (msg.text.equals("/lampu_kamar3")){
        stateK3 = !stateK3;
        digitalWrite(lampuK3,stateK3);
        Blynk.virtualWrite(virtualK3,stateK3);
        bot.sendMessage(user_id,"success5");
      }else{
        bot.sendMessage(user_id,"unknown command");
      }
    }
  }
  
  Blynk.run();
  timer.run();
}

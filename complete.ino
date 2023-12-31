#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6eCMWr2VA"
#define BLYNK_TEMPLATE_NAME "Test"
#define BLYNK_AUTH_TOKEN "8x_EcDrGAWFZCY3kU6u71xaDRbO8W8Cw"

#include <CTBot.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <DHT.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;

#define BOTtoken "6741323250:AAF6Ag62a6mbxs0IsmN9IcCF2NB4FSE6xDw"
#define user_id // isi user id telegram, contoh id 5194206843

CTBot bot;

const char* ssid = "wifi";
const char* pass = "pass";
// esp32 pin used
#define pinLDR 34
#define pinDHT 23
#define pinAPI 22
#define pinGAS 36
#define pinDoor 39

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

#define lampuTer 15
#define pinKipas 2
#define pinAlarm 4
// virtual pin
#define virtualRU V0
#define virtualK1 V1
#define virtualKM V2
#define virtualHall V3
#define virtualK2 V4
#define virtualK3 V5

#define virtualTer V6
#define virtualLDR V7
#define virtualVolt V8
#define virtualRaw V9

#define virtualKipas V10
#define virtualKS V11
#define virtualTemp V12
#define virtualHum V13

#define virtualDoor V14
#define virtualSD V15

#define virtualAPI V16
#define virtualGAS V17
// state variable
int stateRU = 0;
int stateK1 = 0;
int stateKM = 0;
int stateHall = 0;
int stateK2 = 0;
int stateK3 = 0;

int stateLDR = 0;
int stateKipas = 0;
int stateDoor = 0;
// other var
#define DHTtype DHT11
DHT dht(pinDHT,DHTtype);

BLYNK_CONNECTED(){
  Blynk.syncVirtual(virtualRU);
  Blynk.syncVirtual(virtualK1);
  Blynk.syncVirtual(virtualKM);
  Blynk.syncVirtual(virtualHall);
  Blynk.syncVirtual(virtualK2);
  Blynk.syncVirtual(virtualK3);

  Blynk.syncVirtual(virtualLDR);
  Blynk.syncVirtual(virtualKS);
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
BLYNK_WRITE(virtualK3){
  stateK3 = param.asInt();
  digitalWrite(lampuK3,stateK3);
}

BLYNK_WRITE(virtualKS){
  stateKipas = param.asInt();
}
BLYNK_WRITE(virtualLDR){
  stateLDR = param.asInt();
}
BLYNK_WRITE(virtualSD){
  stateDoor = param.asInt();
}

void setup(){
  Serial.begin(9600);

  bot.wifiConnect(ssid,pass);
  bot.setTelegramToken(BOTtoken);
  Blynk.begin(auth,ssid,pass);
  dht.begin();

  pinMode(lampuRU,OUTPUT);
  pinMode(lampuK1,OUTPUT);
  pinMode(lampuKM,OUTPUT);
  pinMode(lampuHall,OUTPUT);
  pinMode(lampuK2,OUTPUT);
  pinMode(lampuK3,OUTPUT);
  pinMode(lampuTer,OUTPUT);
  pinMode(pinKipas,OUTPUT);
  pinMode(pinAlarm,OUTPUT);

  pinMode(pinLDR,INPUT);
  pinMode(pinDoor,INPUT);
  pinMode(pinGAS,INPUT);
  pinMode(pinAPI,INPUT);

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
  // Security
  int Api = digitalRead(pinAPI);
  int Gas = digitalRead(pinGAS);
  int Door = digitalRead(pinDoor);

// ERROR DISINI
  if (Api==HIGH){
    digitalWrite(pinAlarm,HIGH);
    bot.sendMessage(user_id,"Ada api");
  }else if (Gas==LOW){
    digitalWrite(pinAlarm,HIGH);
    bot.sendMessage(user_id,"Ada gas");
  }else if (Door==HIGH && stateDoor==HIGH){
    digitalWrite(pinAlarm,HIGH);
    bot.sendMessage(user_id,"pintu terbuka");
  }else{
    digitalWrite(pinAlarm,LOW);
  }
// SAMPAI SINI

  Blynk.virtualWrite(virtualAPI,pinAPI);
  Blynk.virtualWrite(virtualGAS,pinGAS);
  Blynk.virtualWrite(virtualDoor,pinDoor);

  // Sensor DHT
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (temp>=28 && stateKipas==HIGH){
    digitalWrite(pinKipas,HIGH);
  }else{
    digitalWrite(pinKipas,LOW);
  }
  Blynk.virtualWrite(virtualKipas,digitalRead(pinKipas));
  Blynk.virtualWrite(virtualTemp,temp);
  Blynk.virtualWrite(virtualHum,hum);

  // Sensor LDR
  const float GAMMA = 0.7;
  const float RL10 = 50;

  float ldrVal = analogRead(pinLDR);
  float voltage = ldrVal / 4096*5;
  float resistance = 2000*voltage/(1-voltage/5);
  float lux = pow(RL10*1e3*pow(18, GAMMA)/resistance, (1/GAMMA));

  if (voltage<0.2 && stateLDR==HIGH){
    digitalWrite(lampuTer,HIGH);
  }else{
    digitalWrite(lampuTer,LOW);
  }
  Blynk.virtualWrite(virtualTer,digitalRead(lampuTer));
  Blynk.virtualWrite(virtualVolt,voltage);
  Blynk.virtualWrite(virtualRaw,ldrVal);

  // Switch
  if (digitalRead(switchRU)==HIGH){
    stateRU = !stateRU;
    digitalWrite(lampuRU,stateRU);
    Blynk.virtualWrite(virtualRU,stateRU);
    delay(200);
  }else if (digitalRead(switchK1)==HIGH){
    stateK1 = !stateK1;
    digitalWrite(lampuK1,stateK1);
    Blynk.virtualWrite(virtualK1,stateK1);
    delay(200);
  }else if (digitalRead(switchKM)==HIGH){
    stateKM = !stateKM;
    digitalWrite(lampuKM,stateKM);
    Blynk.virtualWrite(virtualKM,stateKM);
    delay(200);
  }else if (digitalRead(switchHall)==HIGH){
    stateHall = !stateHall;
    digitalWrite(lampuHall,stateHall);
    Blynk.virtualWrite(virtualHall,stateHall);
    delay(200);
  }else if (digitalRead(switchK2)==HIGH){
    stateK2 = !stateK2;
    digitalWrite(lampuK2,stateK2);
    Blynk.virtualWrite(virtualK2,stateK2);
    delay(200);
  }else if (digitalRead(switchK3)==HIGH){
    stateK3 = !stateK3;
    digitalWrite(lampuK3,stateK3);
    Blynk.virtualWrite(virtualK3,stateK3);
    delay(200);
  }

  // Telegram
  TBMessage msg;

  if (CTBotMessageText == bot.getNewMessage(msg)){
    if (msg.sender.id == user_id){
      if (msg.text.equals("/start")){
        String pesanStart = "Selamat datang"+msg.sender.username+
                            "\n\nBerikut daftar perintah:"+
                            "\n- /start - Daftar perintah"+
                            "\n- /cek_lampu - Keadaan lampu"+
                            "\n- /cek_data - Daftar data"+
                            "\n- /lampu_utama - Atur lampu utama"+
                            "\n- /lampu_kamar1 - Atur lampu kamar 1"+
                            "\n- /lampu_kmandi - Atur lampu kamar mandi"+
                            "\n- /lampu_hall - Atur lampu hall lt.2"+
                            "\n- /lampu_kamar2 - Atur lampu kamar 2 lt.2"+
                            "\n- /lampu_kamar3 - Atur lampu kamar 3 lt.3"+
                            "\n- /state_ldr - Atur keadaan ldr"+
                            "\n- /state_door - Atur keadaan alarm pintu";
        bot.sendMessage(user_id,pesanStart);
        delay(1000);
      }else if (msg.text.equals("/cek_lampu")){
        String cekLampu = "Keadaan lampu: (1=ON - 0=OFF)\n- Lampu Utama        : "+String(digitalRead(lampuRU))+
                          "\n- Lampu Kamar 1      : "+String(digitalRead(lampuK1))+
                          "\n- Lampu Kamar Mandi  : "+String(digitalRead(lampuKM))+
                          "\n- Lampu Hall (lt2)   : "+String(digitalRead(lampuHall))+
                          "\n- Lampu Kamar 2 (lt2): "+String(digitalRead(lampuK2))+
                          "\n- Lampu Kamar 3 (lt2): "+String(digitalRead(lampuK3));
        bot.sendMessage(user_id,cekLampu);
        delay(1000);
      }else if (msg.text.equals("/cek_data")){
        String cekData = "Data DHT:\n- Suhu       : "+String(temp)+"C"+
                          "\n- Kelembapan : "+String(hum)+"%"+
                          "\n\n Data LDR:"+
                          "\n- Raw        : "+String(ldrVal)+
                          "\n- Voltage    : "+String(voltage)+"V"+
                          "\n- Lux        : "+String(lux);
        bot.sendMessage(user_id,cekData);
        delay(1000);
      }else if (msg.text.equals("/lampu_utama")){
        stateRU = !stateRU;
        digitalWrite(lampuRU,stateRU);
        Blynk.virtualWrite(virtualRU,stateRU);
        bot.sendMessage(user_id,"success");
        delay(200);
      }else if (msg.text.equals("/lampu_kamar1")){
        stateK1 = !stateK1;
        digitalWrite(lampuK1,stateK1);
        Blynk.virtualWrite(virtualK1,stateK1);
        bot.sendMessage(user_id,"success1");
        delay(200);
      }else if (msg.text.equals("/lampu_kmandi")){
        stateKM = !stateKM;
        digitalWrite(lampuKM,stateKM);
        Blynk.virtualWrite(virtualKM,stateKM);
        bot.sendMessage(user_id,"success2");
        delay(200);
      }else if (msg.text.equals("/lampu_hall")){
        stateHall = !stateHall;
        digitalWrite(lampuHall,stateHall);
        Blynk.virtualWrite(virtualHall,stateHall);
        bot.sendMessage(user_id,"success3");
        delay(200);
      }else if (msg.text.equals("/lampu_kamar2")){
        stateK2 = !stateK2;
        digitalWrite(lampuK2,stateK2);
        Blynk.virtualWrite(virtualK2,stateK2);
        bot.sendMessage(user_id,"success4");
        delay(200);
      }else if (msg.text.equals("/lampu_kamar3")){
        stateK3 = !stateK3;
        digitalWrite(lampuK3,stateK3);
        Blynk.virtualWrite(virtualK3,stateK3);
        bot.sendMessage(user_id,"success5");
        delay(200);
      }else if (msg.text.equals("/state_ldr")){
        stateLDR = !stateLDR;
        Blynk.virtualWrite(virtualLDR,stateLDR);
        bot.sendMessage(user_id,"successLDR");
        delay(200);
      }else if (msg.text.equals("/state_door")){
        stateDoor = !stateDoor;
        Blynk.virtualWrite(virtualSD,stateDoor);
        bot.sendMessage(user_id,"successDoor");
        delay(200);
      }else{
        bot.sendMessage(user_id,"unknown command");
        delay(200);
      }
    }
  }
  
  Blynk.run();
}

// BLYNK Setup
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6VP7zHhbO"
#define BLYNK_TEMPLATE_NAME "PROJECT TECH1"
#define BLYNK_AUTH_TOKEN "aLI6cOE9eubSCW2P4hbwyvkRZ9cAg4Wk"
//library BLYNK
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;
BlynkTimer timer;

// Setup telegram bot
#define BOTtoken "6741323250:AAF6Ag62a6mbxs0IsmN9IcCF2NB4FSE6xDw"
#define user_id 5194206843
//library Telegram
#include <CTBot.h>
#include <ArduinoJson.h>

CTBot bot;

// Library
#include <DHT.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

// Setup koneksi WiFi
const char* ssid = "lalala";
const char* pass = "00000000";

WiFiClientSecure client;

// Pin Sensor
#define MQ2PIN 36 // Sensor MQ2 Digital
#define sensorApi 39 // Sensor Api
#define DHTPIN 34 // Sensor DHT11
#define LDRPIN 35 // Sensor LDR Cahaya
#define MagnetSW 32 // Magnet Switch 1
// Lantai 1
#define swRU 13
#define swK1 12
#define swKM 14
// Lantai 2
#define swHall 27
#define swK2 26
#define swK3 25

// Pin OUTPUT unfinished
#define lampuHal1 15
#define lampuHal2 2
#define lampuHal3 4
#define lampuRU 16
#define lampuK1 17
#define lampuKM 5
// Lantai 2
#define lampuHall 18
#define lampuK2 19
#define lampuK3 21

#define pinAlarm 3
#define pinKipas 1

// Setup DHT
#define DHTType DHT11
DHT dht(DHTPIN,DHTType);

//variable state
int lampuRUstate = 0;
int lampuK1state = 0;
int lampuKMstate = 0;

int lampuHallstate = 0;
int lampuK2state = 0;
int lampuK3state = 0;

int autoLamp = 1;
int doorAlarm = 1;
int modeKipas = 1;

// Virtual Pin Blynk
#define vpinTemp V0
#define vpinHum V1
#define vpinRaw V2
#define vpinVolt V3
#define vpinLux V4
#define vLampuHal V5

// Virtual Alarm Pin Blynk
#define vMQ2 V6
#define vApi V7
#define vAlarm V8

// Virtual Control Pin Blynk
#define vDoorAlarm V9
#define vModekipas V10
#define vModeAutoLamp V11

#define vLampuRU V12
#define vLampuK1 V13
#define vLampuKM V14

#define vLampuHall V15
#define vLampuK2 V16
#define vLampuK3 V17

BLYNK_CONNECTED(){
  Blynk.syncVirtual(vDoorAlarm);
  Blynk.syncVirtual(vModekipas);
  Blynk.syncVirtual(vModeAutoLamp);

  Blynk.syncVirtual(vLampuRU);
  Blynk.syncVirtual(vLampuK1);
  Blynk.syncVirtual(vLampuKM);

  Blynk.syncVirtual(vLampuHall);
  Blynk.syncVirtual(vLampuK2);
  Blynk.syncVirtual(vLampuK3);
}

BLYNK_WRITE(vDoorAlarm){
  doorAlarm = param.asInt();
}
BLYNK_WRITE(vModeKipas){
  modeKipas = param.asInt();
}
BLYNK_WRITE(vModeAutoLamp){
  autoLamp = param.asInt();
}

BLYNK_WRITE(vLampuRU){
  lampuRUstate = param.asInt();
  digitalWrite(lampuRU,lampuRUstate);
}
BLYNK_WRITE(vLampuK1){
  lampuK1state = param.asInt();
  digitalWrite(lampuK1,lampuK1state);
}
BLYNK_WRITE(vLampuKM){
  lampuKMstate = param.asInt();
  digitalWrite(lampuKM,lampuKMstate);
}

BLYNK_WRITE(vLampuHall){
  lampuHallstate = param.asInt();
  digitalWrite(lampuHall,lampuHallstate);
}
BLYNK_WRITE(vLampuK2){
  lampuK2state = param.asInt();
  digitalWrite(lampuK2,lampuK2state);
}
BLYNK_WRITE(vLampuK3){
  lampuK2state = param.asInt();
  digitalWrite(lampuK3,lampuK3state);
}

void setup() {
  Serial.begin(9600);
  
  // Setup
  dht.begin(); // DHT
  bot.wifiConnect(ssid,pass); // WiFi
  bot.setTelegramToken(BOTtoken); // Telegram
  Blynk.begin(auth,ssid,pass); // Blynk

  // pin input
  pinMode(MQ2PIN, INPUT);
  pinMode(sensorApi, INPUT);
  pinMode(LDRPIN, INPUT);
  pinMode(MagnetSW, INPUT);

  pinMode(swRU, INPUT_PULLUP);
  pinMode(swK1, INPUT_PULLUP);
  pinMode(swKM, INPUT_PULLUP);

  // pin output
  pinMode(lampuHal1, OUTPUT);
  pinMode(lampuHal2, OUTPUT);
  pinMode(lampuRU, OUTPUT);
  pinMode(lampuK1, OUTPUT);
  pinMode(lampuKM, OUTPUT);

  pinMode(pinAlarm, OUTPUT);
  pinMode(pinKipas, OUTPUT);

  // Setup MQ2
  delay(20000);

  if (bot.testConnection()){
		Serial.println("\nConnecting success");
    bot.sendMessage(user_id,"Bot siap digunakan. /start");
  }else {
		Serial.println("\nConnecting fail, please reset");
    bot.sendMessage(user_id,"Gagal terhubung, silakan reset");
  }
}

void loop() {
  // Pembacaan sensor MQ2
  int keadaanGas = digitalRead(MQ2PIN);
  Serial.println("Keadaan Sensor Gas");
  if (keadaanGas == HIGH){
    digitalWrite(pinAlarm,LOW);
  }else{
    bot.sendMessage(user_id,"Ada gas!!!");
    digitalWrite(pinAlarm,HIGH);
  }

  // Pembacaan sensor Api
  int api = digitalRead(sensorApi);
  Serial.println("Keadaan Sensor Api: ");
  if (api == HIGH){
    bot.sendMessage(user_id,"Ada api!!!");
    digitalWrite(pinAlarm,HIGH);
  }else{
    digitalWrite(pinAlarm,LOW);
  }

  // Pembacaan Magnet Switch
  if (digitalRead(MagnetSW) == HIGH && doorAlarm == HIGH){
    bot.sendMessage(user_id,"Pintu terbuka!!!"); // Test telegram
    digitalWrite(pinAlarm,HIGH);
    Serial.println("Alarm ON");
  }else {
    digitalWrite(pinAlarm,LOW);
    Serial.println("Alarm OFF");
  }

  // Pembacaan sensor DHT11
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  // Kipas
  if (temp>=28 && modeKipas==HIGH){
    Serial.println("Kipas Menyala");
    digitalWrite(pinKipas,HIGH);
  }else{
    Serial.println("Kipas Mati");
    digitalWrite(pinKipas,LOW);
  }

  // Pembacaan Sensor LDR
  const float GAMMA = 0.7;
  const float RL10 = 50;

  float ldrVal = analogRead(LDRPIN);
  float voltage = ldrVal / 4096*5;
  float resistance = 2000*voltage/(1-voltage/5);
  float lux = pow(RL10*1e3*pow(18, GAMMA)/resistance, (1/GAMMA));

  if (voltage<0.7 && autoLamp == HIGH){
    digitalWrite(lampuHal1,HIGH);
  }else{
    digitalWrite(lampuHal1,LOW);
  }

  //BLYNK Update
  // DHT
  Blynk.virtualWrite(vpinTemp, temp);
  Blynk.virtualWrite(vpinHum, hum);
  // LDR
  Blynk.virtualWrite(vpinRaw,ldrVal);
  Blynk.virtualWrite(vpinVolt,voltage);
  Blynk.virtualWrite(vpinLux,lux);
  Blynk.virtualWrite(vLampuHal,digitalRead(lampuHal1));
  
  // Setting Telegram
  TBMessage msg;

  if (CTBotMessageText == bot.getNewMessage(msg)){ // Memeriksa id pengirim pesan
    if (msg.sender.id == user_id){

      if (msg.text.equals("/start")){
        String pesanMasuk = "Selamat Datang, "+msg.sender.username+
                            ".\n\nBerikut daftar perintah yang dapat digunakan:"+ 
                            "\n/start - pesan masuk dan daftar perintah."+
                            "\n/door_alarm - mengubah mode alarm (on/off)."+
                            "\n/mode_kipas - mengubah mode otomasi kipas (on/off)"+
                            "\n/cek_status - cek keadaan lampu dirumah dan kipas."+
                            "\n/cek_data - cek suhu dan kelembapan saat ini.";
        bot.sendMessage(user_id, pesanMasuk);

      }else if (msg.text.equals("/lampu_auto")){
        autoLamp = !autoLamp;
        bot.sendMessage(user_id,"mode lampu otomatis beralih menjadi: "+String(autoLamp)+"(default 1");

      }else if (msg.text.equals("/door_alarm")){
        doorAlarm = !doorAlarm;
        bot.sendMessage(user_id,"mode alarm beralih menjadi: "+String(doorAlarm)+"(default 1)");

      }else if(msg.text.equals("/mode_kipas")){
        modeKipas = !modeKipas;
        bot.sendMessage(user_id,"mode kipas beralih menjadi: "+String(modeKipas)+"(default 1)");

      }else if(msg.text.equals("/lampu_utama")){
        control_rly(0);
        Blynk.virtualWrite(vLampuRU,lampuRUstate);
        bot.sendMessage(user_id,"lampu sekarang dalam keadaan"+String(lampuRU));

      }else if(msg.text.equals("/lampu_kamar-1")){
        control_rly(1);
        Blynk.virtualWrite(vLampuK1,lampuK1state);
        bot.sendMessage(user_id,"lampu sekarang dalam keadaan"+String(lampuK1));

      }else if(msg.text.equals("/lampu_kamar-mandi")){
        control_rly(2);
        Blynk.virtualWrite(vLampuKM,lampuKMstate);
        bot.sendMessage(user_id,"lampu sekarang dalam keadaan"+String(lampuKM));

      }else if(msg.text.equals("/lampu_hall")){
        control_rly(0);
        Blynk.virtualWrite(vLampuHall,lampuHallstate);
        bot.sendMessage(user_id,"lampu sekarang dalam keadaan"+String(lampuHall));

      }else if(msg.text.equals("/lampu_kamar-2")){
        control_rly(0);
        Blynk.virtualWrite(vLampuK2,lampuK2state);
        bot.sendMessage(user_id,"lampu sekarang dalam keadaan"+String(lampuK2));

      }else if(msg.text.equals("/lampu_kamar-3")){
        control_rly(0);
        Blynk.virtualWrite(vLampuK3,lampuK3state);
        bot.sendMessage(user_id,"lampu sekarang dalam keadaan"+String(lampuK3));

      }else if(msg.text.equals("/cek_lampu")){
        String lampuStatus = "Lantai 1: (1 = menyala, 2 = mati)\n- Lampu teras depan: "+String(digitalRead(lampuHal1))+
                              "\n- Ruang Utama: "+String(digitalRead(lampuRU))+
                              "\n- Kamar 1: "+String(digitalRead(lampuK1))+
                              "\n- Kamar Mandi"+String(digitalRead(lampuKM))+
                              "\n\nLantai 2:\n- Hall: "+String(digitalRead(lampuHall))+
                              "\n- Kamar 2: "+String(digitalRead(lampuK2))+
                              "\n- Kamar 3: "+String(digitalRead(lampuK3));
        bot.sendMessage(user_id,lampuStatus);

      }else if (msg.text.equals("/cek_status")){  // Menampilkan keadaan rumah
        String cekStatus ="\n- Mode alarm: "+String(doorAlarm)+"(default 1)"+
                          "\n- Mode Kipas: "+String(modeKipas)+"(default 1)";
        bot.sendMessage(user_id,cekStatus);

      }else if (msg.text.equals("/cek_data")){ // Menampilkan suhu dan kelembapan saat ini
        String pesanData = "Data DHT\nSuhu: "+String(temp)+"C"+
                            "\nKelembapan: "+String(hum)+"%"+
                            "\n\nData LDR\nMentah: "+String(ldrVal)+
                            "\nTegangan: "+String(voltage)+
                            "\nHambatan: "+String(lux);
        bot.sendMessage(user_id, pesanData);

      }else{
        bot.sendMessage(user_id, "Maaf, perintah tidak dikenal.\nSilakan gunakan perintah /start untuk melihat daftar perintah.");

      }
    }else { // Jika id pengirim tidak sesuai dengan id yg ada
      bot.sendMessage(msg.sender.id, "Maaf, anda tidak memiliki izin.");
    }
  }

  button_ctrl();

  delay(100);
}

void button_ctrl(){
  if(digitalRead(swRU)==0){
    control_rly(0);
    Blynk.virtualWrite(vLampuRU,lampuRUstate);
  }else if(digitalRead(swK1)==0){
    control_rly(1);
    Blynk.virtualWrite(vLampuK1,lampuK1state);
  }else if(digitalRead(swKM)==0){
    control_rly(2);
    Blynk.virtualWrite(vLampuKM,lampuKMstate);
  }
  else if (digitalRead(swHall)==0){
    control_rly(3);
    Blynk.virtualWrite(vLampuHall,lampuHallstate);
  }else if(digitalRead(swK2)==0){
    control_rly(4);
    Blynk.virtualWrite(vLampuK2,lampuK2state);
  }else if(digitalRead(swK3)==0){
    control_rly(5);
    Blynk.virtualWrite(vLampuK3,lampuK3state);
  }
}

void control_rly(int rly){
  if(rly==0){
    lampuRUstate = !lampuRUstate;
    digitalWrite(lampuRU,lampuRUstate);
  }else if(rly==1){
    lampuK1state = !lampuK1state;
    digitalWrite(lampuK1,lampuK1state);
  }else if(rly==2){
    lampuKMstate = !lampuKMstate;
    digitalWrite(lampuKM,lampuKMstate);
  }
  else if (rly==3){
    lampuHallstate = !lampuHallstate;
    digitalWrite(lampuHall,lampuHallstate);
  }else if(rly==4){
    lampuK2state = !lampuK2state;
    digitalWrite(lampuK2,lampuK2state);
  }else if(rly=5){
    lampuK3state = !lampuK3state;
    digitalWrite(lampuK3,lampuK3state);
  }
}

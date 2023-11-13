// BLYNK Setup
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6VP7zHhbO"
#define BLYNK_TEMPLATE_NAME "PROJECT TECH1"
#define BLYNK_AUTH_TOKEN "aLI6cOE9eubSCW2P4hbwyvkRZ9cAg4Wk"
//library
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;
BlynkTimer timer;

// Setup telegram bot
#define BOTtoken "6741323250:AAF6Ag62a6mbxs0IsmN9IcCF2NB4FSE6xDw"
#define user_id 5194206843
//library
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

// Virtual Pin Blynk
#define vpinTemp V0
#define vpinHum V1
#define vpinRaw V2
#define vpinVolt V3
#define vpinLux V4
#define vLampuTeras V5

// Pin Sensor
#define MQ2PIN 14 // Sensor MQ2 Digital
#define sensorApi 27 // Sensor Api
#define DHTPIN 26 // Sensor DHT11
#define LDRPIN 34 // Sensor LDR Cahaya
#define MagnetSw1 25 // Magnet Switch 1

// Pin Output
#define alarmMQ2 15 // Alarm Gas MQ2
#define alarmApi 2 // Alarm Api
#define alarmSw 23 // Alarm Magnet Switch

#define pinKipas 4 // Kipas

#define lampuHal 5 // Lampu Halaman

// Pin Lampu
/* unfinished
#define LampuRT
#define LampuK1
#define LampuKM
#define lampuD
#define lampuLT2
#define lampuK2
#define lampuK3

#define Panel1
#define Panel2
*/

// Variable
#define DHTType DHT11
DHT dht(DHTPIN,DHTType);

int MagnetTimer = 15;
int MagnetNotif = HIGH;

int modeKipas = HIGH;

void setup() {
  Serial.begin(9600);
  

  // Setup
  dht.begin(); // DHT
  bot.wifiConnect(ssid,pass); // WiFi
  bot.setTelegramToken(BOTtoken); // Telegram
  Blynk.begin(auth,ssid,pass); // Blynk

  // Mode Pin INPUT
  pinMode(MQ2PIN, INPUT); // Sensor MQ2
  pinMode(sensorApi, INPUT); // Sensor Api
  pinMode(MagnetSw1, INPUT);

  // Mode Pin OUTPUT
  pinMode(alarmMQ2, OUTPUT); // ALARM MQ2 GAS
  pinMode(alarmApi, OUTPUT); // ALARM API
  pinMode(alarmSw, OUTPUT);

  pinMode(pinKipas, OUTPUT); // KIPAS ANGIN

  pinMode(lampuHal, OUTPUT); // Lampu Halaman

  // Setup MQ2
  delay(20000);
  Serial.println("Sensor MQ2 siap");

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
    Serial.println("Tidak ada gas");
    digitalWrite(alarmMQ2,LOW);
  }else{
    Serial.println("Ada gas");
    bot.sendMessage(user_id,"Ada gas!!!");
    digitalWrite(alarmMQ2,HIGH);
  }

  // Pembacaan sensor Api
  int api = digitalRead(sensorApi);
  Serial.println("Keadaan Sensor Api: ");
  if (api == HIGH){
    bot.sendMessage(user_id,"Ada api!!!");
    Serial.println("Ada api");
    digitalWrite(alarmApi,HIGH);
  }else{
    Serial.println("Tidak ada api");
    digitalWrite(alarmApi,LOW);
  }

  // Pembacaan Magnet Switch
  if (digitalRead(MagnetSw1) == HIGH && MagnetNotif == HIGH){
    bot.sendMessage(user_id,"Pintu terbuka!!!"); // Test telegram
    digitalWrite(alarmSw,HIGH);
    Serial.println("Alarm ON");
  }else {
    digitalWrite(alarmSw,LOW);
    Serial.println("Alarm OFF");
  }

  // Pembacaan sensor DHT11
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  // Blynk Update
  Blynk.virtualWrite(vpinTemp, temp);
  Blynk.virtualWrite(vpinHum, hum);
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

  String Halaman;
  if (voltage<0.7){
    digitalWrite(lampuHal,HIGH);
    Halaman = "Nyala";
  }else{
    digitalWrite(lampuHal,LOW);
    Halaman = "Mati";
  }
  //BLYNK Update
  Blynk.virtualWrite(vpinRaw,ldrVal);
  Blynk.virtualWrite(vpinVolt,voltage);
  Blynk.virtualWrite(vpinLux,lux);
  Blynk.virtualWrite(vLampuTeras,digitalRead(lampuHal));

  Serial.println();
  
  // Setting Telegram
  TBMessage msg;

  if (CTBotMessageText == bot.getNewMessage(msg)){ // Memeriksa id pengirim pesan
    if (msg.sender.id == user_id){

      if (msg.text.equals("/start")){
        String pesanMasuk = "Selamat Datang, "+msg.sender.username+".";
        pesanMasuk += "\n\nBerikut daftar perintah yang dapat digunakan: ";
        pesanMasuk += "\n/start - pesan masuk dan daftar perintah.";
        pesanMasuk += "\n/mode_alarm - mengubah mode alarm (on/off).";
        pesanMasuk += "\n/mode_kipas - mengubah mode otomasi kipas (on/off)";
        pesanMasuk += "\n/cek_status - cek keadaan lampu dirumah dan kipas.";
        pesanMasuk += "\n/cek_data - cek suhu dan kelembapan saat ini.";
        bot.sendMessage(user_id, pesanMasuk);

      }else if (msg.text.equals("/mode_alarm")){
        MagnetNotif = !MagnetNotif;
        bot.sendMessage(user_id,"mode alarm beralih menjadi: "+String(MagnetNotif)+"(default 1)");

      }else if(msg.text.equals("/mode_kipas")){
        modeKipas = !modeKipas;
        bot.sendMessage(user_id,"mode kipas beralih menjadi: "+String(modeKipas)+"(default 1)");

      }else if (msg.text.equals("/cek_status")){  // Menampilkan keadaan rumah
        String cekStatus = "- Lampu teras depan: "+Halaman;
        cekStatus += "\n- Mode alarm: "+String(MagnetNotif)+"(default 1)";
        cekStatus += "\n- Mode Kipas: "+String(modeKipas)+"(default 1)";
        bot.sendMessage(user_id,cekStatus);

      }else if (msg.text.equals("/cek_data")){ // Menampilkan suhu dan kelembapan saat ini
        String pesanData = "Data DHT";
        pesanData += "\nSuhu: "+String(temp)+"C";
        pesanData += "\nKelembapan: "+String(hum)+"%";
        pesanData += "\n\nData LDR";
        pesanData += "\nMentah: "+String(ldrVal);
        pesanData += "\nTegangan: "+String(voltage);
        pesanData += "\nHambatan: "+String(lux);
        bot.sendMessage(user_id, pesanData);

      }else{
        bot.sendMessage(user_id, "Maaf, perintah tidak dikenal.\nSilakan gunakan perintah /start untuk melihat daftar perintah.");

      }
    }else { // Jika id pengirim tidak sesuai dengan id yg ada
      bot.sendMessage(msg.sender.id, "Maaf, anda tidak memiliki izin.");
    }
  }

  delay(1000);
}

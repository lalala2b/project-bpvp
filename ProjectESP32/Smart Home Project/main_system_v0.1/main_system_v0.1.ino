// Library
#include <DHT.h>

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

// Variable
#define DHTType DHT11
DHT dht(DHTPIN,DHTType);

void setup() {
  Serial.begin(9600);

  // Setup
  dht.begin();

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
    digitalWrite(alarmMQ2,HIGH);
  }

  // Pembacaan sensor Api
  int api = digitalRead(sensorApi);
  Serial.println("Keadaan Sensor Api: ");
  if (api == HIGH){
    Serial.println("Ada api");
    digitalWrite(alarmApi,HIGH);
  }else{
    Serial.println("Tidak ada api");
    digitalWrite(alarmApi,LOW);
  }

  // Pembacaan Magnet Switch
  int kondisiMagnet1 = digitalRead(MagnetSw1);

  if (kondisiMagnet1 == HIGH){
    digitalWrite(alarmSw,HIGH);
    Serial.println("Alarm ON");
  }else {
    digitalWrite(alarmSw,LOW);
    Serial.println("Alarm OFF");
  }

  // Pembacaan sensor DHT11
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  Serial.println("Pembacaan Suhu & Kelembapan: ");
  Serial.println("Suhu: "+String(temp)+"C");
  Serial.println("Kelembapan: "+String(hum)+"%");
  if (temp>=28){
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

  Serial.println("Pembacaan sensor LDR");
  Serial.println("Raw: "+String(ldrVal));
  Serial.println("Voltage: "+String(voltage));
  Serial.println("Hambatan: "+String(lux));

  if (voltage<0.7){
    digitalWrite(lampuHal,HIGH);
  }else{
    digitalWrite(lampuHal,LOW);
  }

  Serial.println();
  delay(1000);
}

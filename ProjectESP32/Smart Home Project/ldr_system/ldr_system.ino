
// Lampu menggunakan LDR
#define LDR 34
const int sensorMin=0;
const int sensorMax=1024;

// PIN Lampu
#define lampu1 18
#define lampu2 19

// Sensor Api
#define sensorApi 35

void setup(){
  Serial.begin(9600);

  // lampu LDR
  pinMode(lampu1,OUTPUT);
  pinMode(lampu2,OUTPUT);
}

void loop(){
  delay(1000);

  // Sensor Api
  int nilaiApi = analogRead(35);
  Serial.println("Api: "+String(nilaiApi));
  int range = map(nilaiApi, sensorMin, sensorMax, 0, 3);
  Serial.println("Range: "+String(range));
  Serial.println();

  // Lampu menggunakan LDR
  const float GAMMA = 0.7;
  const float RL10 = 50;

  float ldrVal = analogRead(LDR);
  float voltage = ldrVal / 4096*5;
  float resistance = 2000*voltage/(1-voltage/5);
  float lux = pow(RL10*1e3*pow(18, GAMMA)/resistance, (1/GAMMA));
  Serial.println(ldrVal);
  Serial.println(voltage);
  Serial.println(lux);
  Serial.println();

  if (ldrVal<100){
    digitalWrite(lampu1,HIGH);
    digitalWrite(lampu2,HIGH);
  }else{
    digitalWrite(lampu1,LOW);
    digitalWrite(lampu2,LOW);
  }
  // Lampu menggunakan LDR
}
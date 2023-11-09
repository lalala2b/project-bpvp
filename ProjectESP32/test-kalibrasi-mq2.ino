#define RL 10
float analogValue = 0;
float VRL = 0;
float Rs = 0;
float Ro = 0;

void setup(){
  pinMode(36,INPUT);
  Serial.begin(9600);
}

void loop(){
  analogValue = analogRead(36);
  VRL = analogValue*(3.3/4095.0);
  Rs = ((3.3/VRL)-1)*RL;
  Ro = Rs/9.6;
  Serial.println("Ro: "+String(Ro));
  delay(1000)
}

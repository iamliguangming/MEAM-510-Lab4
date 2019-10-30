#define 1A 21;
#define 2A 22;
#define Enable 4;
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
pinMode(1A,OUTPUT);
pinMode(2A,OUTPUT)；
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(1A,HIGH);
  digitalWrite(2A,LOW);
  digitalWrite(Enable,HIGH);
  delay(10);
  digitalWrite(Enable,LOW);
  delay(10);
  
}

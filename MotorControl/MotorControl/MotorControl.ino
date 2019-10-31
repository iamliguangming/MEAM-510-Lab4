const int A1= 21;
const int A2 =22;
const int Enable=4;
const int EnableControl = 32;
const int FrontorBack = 33;
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
pinMode(A1,OUTPUT);
pinMode(A2,OUTPUT);
pinMode(Enable,OUTPUT);
pinMode(EnableControl,INPUT);
pinMode(FrontorBack,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int val = map(analogRead(EnableControl),0,4095,0,100);
  Serial.println(val);
  if (digitalRead(FrontorBack)==HIGH){
  digitalWrite(A1,HIGH);
  digitalWrite(A2,LOW);
  }
  else if (digitalRead(FrontorBack)==LOW)
  {
  digitalWrite(A1,LOW);
  digitalWrite(A2,HIGH);
  }
  digitalWrite(Enable,HIGH);
  delay((5*val)/100);
  digitalWrite(Enable,LOW);
  delay(5-(5*val)/100);
  
}

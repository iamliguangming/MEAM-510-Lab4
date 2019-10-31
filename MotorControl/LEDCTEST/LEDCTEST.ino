const int LEDC_CHANNEL = 0;
const int LEDC_RESOLUTION_BITS=13;
const int LEDC_RESOLUTION = ((1<<LEDC_RESOLUTION_BITS)-1);
const int LEDC_FREQ_HZ = 5000;
const int A1= 21;
const int A2 =22;
const int Enable=4;
const int EnableControl = 32;
const int FrontorBack = 33;
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
ledcSetup(LEDC_CHANNEL,LEDC_FREQ_HZ,LEDC_RESOLUTION_BITS);
ledcAttachPin(Enable,LEDC_CHANNEL);
pinMode(A1,OUTPUT);
pinMode(A2,OUTPUT);
pinMode(EnableControl,INPUT);
pinMode(FrontorBack,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int val = map(analogRead(EnableControl),0,4095,0,255);
  uint32_t duty = LEDC_RESOLUTION*min(val,255)/255;
  ledcWrite(LEDC_CHANNEL,duty);
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
  
}

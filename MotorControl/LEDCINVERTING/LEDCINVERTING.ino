const int LEDC_CHANNEL = 0;
const int LEDC_CHANNEL1 = 1;
const int LEDC_RESOLUTION_BITS=13;
const int LEDC_RESOLUTION = ((1<<LEDC_RESOLUTION_BITS)-1);
const int LEDC_FREQ_HZ = 5000;
const int A1= 21;
const int A44 =22;
const int Enable=4;
const int Enable1 = 0;
const int EnableControl = 32;
const int FrontorBack = 33;
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
ledcSetup(LEDC_CHANNEL,LEDC_FREQ_HZ,LEDC_RESOLUTION_BITS);
ledcSetup(LEDC_CHANNEL1,LEDC_FREQ_HZ,LEDC_RESOLUTION_BITS);
ledcAttachPin(Enable,LEDC_CHANNEL);
ledcAttachPin(Enable1,LEDC_CHANNEL1);
pinMode(A1,OUTPUT);
pinMode(A44,OUTPUT);
pinMode(EnableControl,INPUT);
pinMode(FrontorBack,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int val = map(analogRead(EnableControl),0,4095,-1000,1000);
  uint32_t duty = LEDC_RESOLUTION*abs(val)/1000;
  Serial.println(val);
  if (val >= 0){
  digitalWrite(A1,HIGH);
  digitalWrite(A44,HIGH);
//  duty = LEDC_RESOLUTION*val/1000;
  }
  else if (val<0)
  {
//  duty = LEDC_RESOLUTION*(10\
00-val)/1000;
  digitalWrite(A1,LOW);
  digitalWrite(A44,LOW);
  }
  ledcWrite(LEDC_CHANNEL,duty);
  ledcWrite(LEDC_CHANNEL1,duty);
  
}

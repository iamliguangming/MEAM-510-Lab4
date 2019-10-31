const int LEDC_CHANNEL = 0;
const int LEDC_RESOLUTION_BITS=13;
const int LEDC_RESOLUTION = ((1<<LEDC_RESOLUTION_BITS)-1); //2^13 -1 = 8191
const int LEDC_FREQ_HZ = 5000; //~41 MHz (resolution*Freq Hz <= 80 MHz
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
  int val = map(analogRead(EnableControl),0,4095,-1000,1000); // what if we did -1000 - 1000, then we only need to calc duty once: ledc_res*abs(val)/1000. At least this gives 0 dutyf
  uint32_t duty = LEDC_RESOLUTION*abs(val)/1000;
  Serial.println(val);
  if (val >= 100){
  digitalWrite(A1,HIGH);
  digitalWrite(A2,LOW);
//  duty = LEDC_RESOLUTION*val/1000;
  }
  else if (val<=-100)
  {
//  duty = LEDC_RESOLUTION*(1000-val)/255;
  digitalWrite(A1,LOW);
  digitalWrite(A2,HIGH);
  }
  else{ // break between -100 to 100
    digitalWrite(A1, HIGH);
    digitalWrite(A2, HIGH);
  }
  ledcWrite(LEDC_CHANNEL,duty);
  
}

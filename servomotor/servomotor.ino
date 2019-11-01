const int LEDC_CHANNEL = 0;
const int LEDC_RESOLUTION_BITS = 13;
const int LEDC_RESOLUTION = ((1<<LEDC_RESOLUTION_BITS)-1);
const int LEDC_FREQ_HZ = 50   ;
const int ServoControl = 23;
const int ServoInput = 32;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  ledcSetup(LEDC_CHANNEL,LEDC_FREQ_HZ,LEDC_RESOLUTION_BITS);
  ledcAttachPin(ServoControl,LEDC_CHANNEL);
  pinMode(ServoControl,OUTPUT);
  pinMode(ServoInput,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int val = map(analogRead(ServoInput),0,4095,0,1000);
  uint32_t duty = LEDC_RESOLUTION*min(val,1000)/1000;
  Serial.println(val);
  ledcWrite(LEDC_CHANNEL,duty);
  

}

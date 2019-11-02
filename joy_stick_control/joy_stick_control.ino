const int joystickX = 32;
const int joystickY = 33;

void setup() {
Serial.begin(115200);
pinMode(joystickX,INPUT);
pinMode(joystickY,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  int x = map(analogRead(joystickX),0,4095,-1000,1000);
  int y = map(analogRead(joystickY),0,4095,-1000,1000);
  Serial.println("X location");
  Serial.println(x);
  Serial.println("Y location");
  Serial.println(y);
  delay(100);

}

const int joystickX = 32;
const int joystickY = 33;
const int joystickZ = 21;
void setup() {
Serial.begin(115200);
pinMode(joystickX,INPUT);
pinMode(joystickY,OUTPUT);
pinMode(joystickZ,INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  int x = map(analogRead(joystickX),0,4095,-1000,1000);
  int y = map(analogRead(joystickY),0,4095,-1000,1000);
  int z = digitalRead(joystickZ);
  Serial.println("X location");
  Serial.println(x);
  Serial.println("Y location");
  Serial.println(y);
  Serial.println("Switch Status");
  Serial.println(z);
  delay(100);

}

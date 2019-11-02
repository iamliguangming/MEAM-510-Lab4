const int joystickX = 32;
const int joystickY = 33;

void setup() {
Serial.begin(115200);
pinMode(joystickX,INPUT);
pinMode(joystickY,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  int x = analogRead(joystickX);
  int y = analogRead(joystickY);
  Serial.println("X location");
  Serial.println(x);
  Serial.println("Y location");
  Serial.println(y);
  delay(100);

}

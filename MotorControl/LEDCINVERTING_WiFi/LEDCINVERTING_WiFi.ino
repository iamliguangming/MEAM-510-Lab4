#include <WiFi.h>;
#include <WiFiUdp.h>;
int port = 1609;
int cb = 0;
int val = 0;
void receivePacket();
int MessageReceived = 0;
const char* ssid = "SmartGuangming";
WiFiUDP  UDPTestServer;
IPAddress myIPaddress(192,168,1,142);
const int UDP_PACKET_SIZE = 100;
byte packetBuffer[UDP_PACKET_SIZE + 1];
char udpBuffer[UDP_PACKET_SIZE];
IPAddress ipTarget(192,168,1,125);

const int LEDC_CHANNEL = 0;
const int LEDC_CHANNEL1 = 1;
const int LEDC_CHANNEL_SERVO = 2;
const int LEDC_RESOLUTION_BITS=13;
const int LEDC_RESOLUTION = ((1<<LEDC_RESOLUTION_BITS)-1);
const int LEDC_FREQ_HZ = 5000;
const int LEDC_FREQ_HZ_SERVO = 50;
const int A1= 21;
const int A44 =22;
const int Enable=4;
const int Enable1 = 0;
const int EnableControl = 32;
const int ServoControl = 33;

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
Serial.println();
Serial.print("Connecting to");
Serial.println(ssid);
WiFi.mode(WIFI_STA);
WiFi.config(myIPaddress,IPAddress(192,168,1,1),IPAddress(255,255,255,0));
WiFi.begin(ssid);
WiFi.setSleep(false);
while (WiFi.status()!=WL_CONNECTED);
{
  delay(500);
  Serial.print(".");
}
Serial.println("WiFi Connected ");
// WiFi.mode(WIFI_AP);
// WiFi.softAP(ssid);
// delay(100);
// WiFi.softAPConfig(myIPaddress,IPAddress(192,168,1,1),IPAddress(255,255,255,0));
 UDPTestServer.begin(port);
packetBuffer[UDP_PACKET_SIZE] = 0;
ledcSetup(LEDC_CHANNEL,LEDC_FREQ_HZ,LEDC_RESOLUTION_BITS);
ledcSetup(LEDC_CHANNEL1,LEDC_FREQ_HZ,LEDC_RESOLUTION_BITS);
ledcSetup(LEDC_CHANNEL_SERVO,LEDC_FREQ_HZ_SERVO,LEDC_RESOLUTION_BITS);
ledcAttachPin(Enable,LEDC_CHANNEL);
ledcAttachPin(Enable1,LEDC_CHANNEL1);
ledcAttachPin(ServoControl,LEDC_CHANNEL_SERVO);
pinMode(A1,OUTPUT);
pinMode(A44,OUTPUT);
pinMode(EnableControl,INPUT);
pinMode(ServoControl,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  receivePacket();
  if (val >= 1000 && val <= 3000)
  {
  int psudoduty = val -2000;
  Serial.println(psudoduty);
  uint32_t duty = LEDC_RESOLUTION*abs(psudoduty)/1000;
  if ((psudoduty) >= 0){
  digitalWrite(A1,HIGH);
  digitalWrite(A44,HIGH);
  Serial.println("Rotating forward");
//  duty = LEDC_RESOLUTION*val/1000;
  }
  else if ((psudoduty)<0)
  {
//  duty = LEDC_RESOLUTION*(1000-val)/1000;
  Serial.println("Rotating backward");
  digitalWrite(A1,LOW);
  digitalWrite(A44,LOW);
  }
  ledcWrite(LEDC_CHANNEL,duty);
  ledcWrite(LEDC_CHANNEL1,duty);
}
  else if (val >= 4000 && val <= 5000)
  {
    uint32_t servoduty = map(val,4000,5000,450,1050)*LEDC_RESOLUTION/10000;
    Serial.println("Servo duty");
    Serial.println(map(val,4000,5000,450,1050));
    ledcWrite(LEDC_CHANNEL_SERVO,servoduty);
  }
  cb = 0;
}

void receivePacket(){
  cb = UDPTestServer.parsePacket();
  if (cb){
    UDPTestServer.read(packetBuffer,UDP_PACKET_SIZE);
    val = (packetBuffer[1]<<8 |packetBuffer[0]);


  }
}

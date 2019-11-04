#include <WiFi.h>;//include WiFi library
#include <WiFiUdp.h>;//include UDP packet
int port = 1609;//Port number 1609 used for communication
int cb = 0;//A flag used to tell if a package is received
int val = 0;//A value used to store analogread value
void receivePacket();//function prototype for receiving packet
int MessageReceived = 0;//declare the messagereceived integer
const char* ssid = "SmartGuangming";//initialize WiFi name
WiFiUDP  UDPTestServer;//Setup UDP protocal
IPAddress myIPaddress(192,168,1,142);//Declare my own IP address
const int UDP_PACKET_SIZE = 100;//declare the packet size to be 100
byte packetBuffer[UDP_PACKET_SIZE + 1];//delcare the an array with size of packet+1
char udpBuffer[UDP_PACKET_SIZE];//declare an array with size of udp packet
IPAddress ipTarget(192,168,1,125);//declare the ipaddress sending packet to

const int LEDC_CHANNEL = 0;//interrupt channel 0
const int LEDC_CHANNEL1 = 1;//interrupt channel 1
const int LEDC_CHANNEL_SERVO = 2;//interrupt channel 2
const int LEDC_RESOLUTION_BITS=13;//bits of resolution is 13
const int LEDC_RESOLUTION = ((1<<LEDC_RESOLUTION_BITS)-1);//resolution is 2^13
const int LEDC_FREQ_HZ = 5000;//interrupt frequency set to be 5000
const int LEDC_FREQ_HZ_SERVO = 50;//interrupt frequency for servo set to be 50
const int A1= 21;//1A used to control the direction on H bridge
const int A44 =22;//4A used to control the NOT direction on H bridge
const int Enable=4;//Enable pin used to control the PWM on H bridge 1
const int Enable1 = 0;//Enable pin used to control PWM on H bridge 2
// const int EnableControl = 32;
// const int ServoControl = 33;

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
// pinMode(EnableControl,INPUT);
// pinMode(ServoControl,OUTPUT);

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

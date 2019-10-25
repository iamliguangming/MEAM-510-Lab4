#include <WiFi.h> //include the wifi file
#include <WiFiUdp.h>//include the UDP protocal file

const char* ssid = "Bozhou_Zha";
const char* password = "19970524";
const int POTPin = 32;
const int LED = 0;
const int Switch = 33;
int val = 0;
int j = 0 ;
void handleUDPServer();
void fncUdpSend();
void fncUdpSendint(int i);
void handleUDPServerint();
WiFiUDP UDPTestServer;
const int UDP_PACKET_SIZE = 100;
char udpBuffer[UDP_PACKET_SIZE];
byte packetBuffer[UDP_PACKET_SIZE+1];
IPAddress ipTarget(192,168,1,191);
IPAddress myIP(192,168,1,142);
//WiFiServer server(80);


void setup() {
pinMode(Switch,INPUT);
pinMode(2,OUTPUT);
pinMode(LED,OUTPUT);
Serial.begin(115200);
Serial.print("Connecting to ");
Serial.println(ssid);
WiFi.mode(WIFI_STA);
WiFi.config(myIP,IPAddress(192,168,1,1),IPAddress(255,255,255,0));
WiFi.begin(ssid,password);
//server.begin();
UDPTestServer.begin(2808);

while (WiFi.status() != WL_CONNECTED)
{
  delay(500);
  Serial.print(".");
}
Serial.println("WiFi connected");
packetBuffer[UDP_PACKET_SIZE] = 0 ;
//Serial.print(WiFi.localIP());
}

//


void loop() {
  // put your main code here, to run repeatedly:

Serial.println(val);
handleUDPServer();
delay(1);
fncUdpSend();
//fncUdpSendint(12345);
//handleUDPServerint();
while (j  <= 49)
{
val = map(analogRead(POTPin),0,4095,100,10000);
Serial.print(!digitalRead(Switch));
digitalWrite(LED,HIGH);
delay(val/100);
j=j+1;
}
j=0;
while ( j <=49)
{
val = map(analogRead(POTPin),0,4095,100,10000);
Serial.print(!digitalRead(Switch));
digitalWrite(LED,LOW);
delay(val/100);
j=j+1;
}
j = 0;
}

void handleUDPServer(){
  int cb = UDPTestServer.parsePacket();
  if (cb) {
    UDPTestServer.read(packetBuffer,UDP_PACKET_SIZE);
    Serial.printf("%s\n",packetBuffer);
  }
}

void fncUdpSend()
{
  strcpy(udpBuffer,"Zhageshuaiye!");
  UDPTestServer.beginPacket(ipTarget, 2808);
  UDPTestServer.printf("%s",udpBuffer);
  UDPTestServer.endPacket();
  Serial.println(udpBuffer);
}
void fncUdpSendint(int i)
{
  udpBuffer[0] = i & 0xff;
  udpBuffer[1] = i >> 8 ;
  udpBuffer[2] = 0;
  UDPTestServer.beginPacket(ipTarget, 2808);
  UDPTestServer.printf("%s",udpBuffer);
  UDPTestServer.endPacket();
}
void handleUDPServerint()
{
  int i, cb = UDPTestServer.parsePacket();
  if (cb)
  {
    UDPTestServer.read(packetBuffer,UDP_PACKET_SIZE);
    i = packetBuffer[0] + packetBuffer[1] << 8;
    Serial.println(i);
  }
}

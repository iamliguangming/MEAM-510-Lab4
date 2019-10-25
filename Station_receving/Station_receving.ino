#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "liguangming";
const char* password = "Iamlegend1";
void handleUDPServer();
void fncUdpSend();
void fncUdpSendint(int i);
void handleUDPServerint();
WiFiUDP UDPTestServer;
const int UDP_PACKET_SIZE = 100;
char udpBuffer[UDP_PACKET_SIZE];
byte packetBuffer[UDP_PACKET_SIZE+1];
IPAddress ipTarget(10,0,0,201);
IPAddress myIP(10,0,0,233);
//WiFiServer server(80);


void setup() {
pinMode(2,OUTPUT);
Serial.begin(115200);
Serial.print("Connecting to ");
Serial.println(ssid);
WiFi.mode(WIFI_STA);
WiFi.config(myIP,IPAddress(10,0,0,1),IPAddress(255,255,255,0));
WiFi.begin(ssid,password);
//server.begin();
UDPTestServer.begin(208);

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


handleUDPServer();
delay(1);
fncUdpSend();
fncUdpSendint(12345);
handleUDPServerint();
digitalWrite(2,HIGH);
delay(1000);
digitalWrite(2,LOW);
delay(1000);
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
  strcpy(udpBuffer,"hello world");
  UDPTestServer.beginPacket(ipTarget, 209);
  UDPTestServer.printf("%s",udpBuffer);
  UDPTestServer.endPacket();
  Serial.println(udpBuffer);
}
void fncUdpSendint(int i)
{
  udpBuffer[0] = i & 0xff;
  udpBuffer[1] = i >> 8 ;
  udpBuffer[2] = 0;
  UDPTestServer.beginPacket(ipTarget, 209);
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

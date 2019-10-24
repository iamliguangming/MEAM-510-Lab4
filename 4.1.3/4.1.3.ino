#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "Liguangming";
const char* password = "2513";


IPAddress myIPaddress(192,168,1,142);
const int UDP_PACKET_SIZE = 100;
byte packetBuffer[UDP_PACKET_SIZE+1];
void handleUDPServer();
WiFiUDP UDPTestServer;
void setup() {
  // put your setup code here, to run once:
WiFi.config(myIPaddress,IPAddress(192,168,1,1),IPAddress(255,255,255,0));
Serial.begin(115200);
WiFi.begin(ssid,password);

UDPTestServer.begin(2808);

while (WiFi.status() != WL_CONNECTED){
  delay(500);
  Serial.print(".");
}

Serial.println("WiFi connected");
packetBuffer[UDP_PACKET_SIZE] = 0;


}

void loop() {
  // put your main code here, to run repeatedly:
handleUDPServer();
delay(1);
}

void handleUDPServer()
{
  int cb = UDPTestServer.parsePacket();
  if (cb)
  {
    UDPTestServer.read(packetBuffer, UDP_PACKET_SIZE);
    Serial.printf("%s\n",packetBuffer);
  }
}

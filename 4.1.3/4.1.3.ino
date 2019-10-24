#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "Liguangming";
const char* password = "2513";


IPAddress myIPaddress(192,168,1,142);
const int UDP_PACKET_SIZE = 100;
byte packetBuffer[UDP_PACKET_SIZE+1];

WiFiUDP UDPTestServer
void setup() {
  // put your setup code here, to run once:
WiFi.config(myIPAdress,IPAddress(192,168,1,1),IPAddress(255,255,255,0));
WiFi.begin(ssid,password);
Serial.begin(115200);
pinMode(2,OUTPUT);
Serial.print("Setting Access Point...");

}

void loop() {
  // put your main code here, to run repeatedly:

}

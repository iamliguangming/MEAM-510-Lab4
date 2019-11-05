#include <WiFi.h> //libraries
#include <WiFiUdp.h>

const int POT = 32; // setting up the pot, led, and switch to the GPIO pins that do not get affected in WIFI
const int SERVOPOT = 33;
void receivePacket();
void sendPacket();

int potread; // initializing the reading from the pot that has been mapped
int cb = 0; // the size of the packet received. 0 if no packet received
int port = 1609; // my local port
int targetPort = 1609; // other player's port
int MessageSent = 0;
int SERVOMessageSent = 0;
int servopotread = 0;
//both for send and receive
const char* ssid = "SmartGuangming"; // my wifi name since I am in AP mode
//const char* password = "GDI";

//WiFiServer server(port);

//for receiver
WiFiUDP UDPTestServer; // setting up UDP communication for receiver
IPAddress myIPaddress (192, 168, 1, 125); //my IP address

const int UDP_PACKET_SIZE = 100; // can be up to 65535
byte packetBuffer [UDP_PACKET_SIZE + 1]; // this will hold the other player's packet

//for sender
WiFiUDP udp; // settinng up UDP communication for sending to the other player
//const int UDP_PACKET_SIZE= 100;
char udpBuffer[UDP_PACKET_SIZE]; // this will hold the packet that I am sending
IPAddress ipTarget(192, 168, 1, 142); //partner's IP address.

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); // baud rate
  // Serial.println();
  // Serial.print("Connecting to ");
  // Serial.println(ssid);
  pinMode(POT, INPUT); // setting up the pot, leds, and switch
  pinMode(2, OUTPUT);
  pinMode(SERVOPOT,INPUT);
  WiFi.setSleep(false);
//
//  //station mode
 // WiFi.mode(WIFI_STA);
 // WiFi.config(myIPaddress, IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0));
 // WiFi.begin(ssid);

    //AP mode
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid);
    WiFi.setSleep(false);
    delay(100);
    WiFi.softAPConfig(myIPaddress, IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0));

  udp.begin(port); // starting the udp communications
  UDPTestServer.begin(port);
//
 // while (WiFi.status() != WL_CONNECTED) {
 //   delay(500);
 //   Serial.print(".");
 // }
 // Serial.println("WiFi connected");
  packetBuffer[UDP_PACKET_SIZE] = 0; // null terminate

}

void loop() {
  potread = map(analogRead(POT),0,4095,3000,1000);
  servopotread = map(analogRead(SERVOPOT),0,4095,4095,5000);
  sendPacket();
  Serial.println(MessageSent);
  }




//
//void receivePacket() { // FOR RECEIVING
//  cb = UDPTestServer.parsePacket(); // parse packet. Sees if I get something
//  if(cb){ // If I did
//    UDPTestServer.read(packetBuffer, UDP_PACKET_SIZE); // read the packet, though it is in bytes
////    Serial.printf("%s\n", packetBuffer);
////    int first = (int)(packetBuffer[0]);
////    int second = (int)(packetBuffer[1]<<8);
//    val = (packetBuffer[1]<<8) | packetBuffer[0]; // convert it to an integer
//    Serial.println(got);
//    cb = 0;
//  }
//}

void sendPacket() {
//  char buf[8];
//  itoa(tenth, buf, 10);
//  strcpy(udpBuffer, buf);// send what ever you want upto buffer size

  udpBuffer[0] = potread & 0xff; // send LSB
  udpBuffer[1] = potread >> 8; // send MSB
  udpBuffer[2] = servopotread  & 0xff;
  udpBuffer[3] = servopotread >> 8;
  udpBuffer[4] = 0; // null terminate

  udp.beginPacket(ipTarget, targetPort);  // send to opponent port
  udp.printf("%s", udpBuffer);
  udp.endPacket(); // end msg


}

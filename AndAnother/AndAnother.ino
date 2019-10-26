#include <WiFi.h> //include the wifi file
#include <WiFiUdp.h>//include the UDP protocal file

const char* ssid = "NANI";
//const char* password = "19970524";
const int POTPin = 32;
const int LED = 0;
const int Switch = 33;
int val = 0;
int j = 0 ;
int cb = 0;
int smallervalue = 0;
int NumberReceived = 0;
//void handleUDPServer();
//void fncUdpSend();
void fncUdpSendint(int i);
void handleUDPServerint();
WiFiUDP UDPTestServer;
const int UDP_PACKET_SIZE = 100;
char udpBuffer[UDP_PACKET_SIZE];
byte packetBuffer[UDP_PACKET_SIZE+1];
IPAddress ipTarget(192,168,1,125);
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
WiFi.begin(ssid);
//server.begin();
UDPTestServer.begin(1609);

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


void loop()
{

while (j<= 49)
{
val = map(analogRead(POTPin),0,4095,100,10000);
Serial.println("val");
Serial.print(val);
digitalWrite(LED,HIGH);
delay(val/100);
if (!digitalRead(Switch) ==1)
{
  fncUdpSendint(val/100);
  Serial.println(val/100);
  while (cb ==0)
  {
  handleUDPServerint();
  }
  cb = 0;
//  NumberReceived = handleUDPServerint();
  Serial.println(NumberReceived);
  smallervalue = (val/100<NumberReceived)?val/100:NumberReceived;
//  if ((val/100) <= NumberReceived)
//  {
//    smallervalue = val/100;
//    
//  }
//  else if ((val/100) > NumberReceived);
//  {
//    smallervalue = NumberReceived;
//  }
  Serial.println("smallervalue");
  Serial.println(smallervalue);
  
  if ((abs(val/100-NumberReceived)*100/smallervalue) < 10)
  {
    while(1)
    {
      digitalWrite(2,HIGH);
    }
  }
  else if ((abs(val/100-NumberReceived)*100/smallervalue) >= 10)
  {
    while(1)
    {
      digitalWrite(2,LOW);
    }

  }
}
handleUDPServerint();
if (cb)
{
//  NumberReceived = handleUDPServerint();
  Serial.println("NumberReceived");
  Serial.println(NumberReceived);
  fncUdpSendint(val/100);
  Serial.println(val/100);
  smallervalue = (val/100<NumberReceived)?val/100:NumberReceived;
// if ((val/100) <= NumberReceived)
//  {
//    smallervalue = val/100;
//  }
//  else if ((val/100) > NumberReceived);
//  {
//    smallervalue = NumberReceived;
//  }
  Serial.println("smallervalue");
  Serial.println(smallervalue);
  if ((abs(val/100-NumberReceived)*100/smallervalue) < 10)
  {
    while(1)
    {
      digitalWrite(2,LOW);
    }
  }
  else if ((abs(val/100-NumberReceived)*100/smallervalue) >= 10)
  {
    while(1)
    {
      digitalWrite(2,HIGH);
    }

  }


}


j=j+1;
}

j=0;


while ( j <=49)
{
val = map(analogRead(POTPin),0,4095,100,10000);
Serial.println("val");
Serial.println(val);

digitalWrite(LED,LOW);
delay(val/100);
if (!digitalRead(Switch) ==1)
{
  fncUdpSendint(val/100);
  Serial.println(val/100);
  while(cb ==0){
//  NumberReceived = handleUDPServerint();
  handleUDPServerint();
  }
  cb  = 0;
  Serial.println(NumberReceived);
  smallervalue = (val/100<NumberReceived)?val/100:NumberReceived;
// if ((val/100 )<= NumberReceived)
//  {
//    smallervalue = val/100;
//  }
//  else if ((val/100) >= NumberReceived);
//  {
//    smallervalue = NumberReceived;
//  }
   Serial.println("smallervalue");
  Serial.println(smallervalue);
  if ((abs(val/100-NumberReceived)*100/smallervalue) < 10)
  {
    while(1)
    {
      digitalWrite(2,HIGH);
    }
  }
  else if (abs((val/100-NumberReceived)*100/smallervalue) >= 10)
  {
    while(1)
    {
      digitalWrite(2,LOW);
    }

  }
}
handleUDPServerint();
if (cb)
{
//   NumberReceived = handleUDPServerint();
   Serial.println(NumberReceived);
  fncUdpSendint(val/100);
  Serial.println(val/100);
  smallervalue = (val/100<NumberReceived)?val/100:NumberReceived;
// if ((val/100)<= NumberReceived)
//  {
//    smallervalue = val/100;
//  }
//  else if ((val/100) >= NumberReceived);
//  {
//    smallervalue = NumberReceived;
//  }
    Serial.println("smallervalue");
  Serial.println(smallervalue);
  if ((abs(val/100-NumberReceived)*100/smallervalue) < 10)
  {
    while(1)
    {
      digitalWrite(2,LOW);
    }
  }
  else if ((abs(val/100-NumberReceived)*100/smallervalue) >= 10)
  {
    while(1)
    {
      digitalWrite(2,HIGH);
    }

  }


}


j=j+1;
}

j=0;
Serial.println();
}

//void handleUDPServer(){
//  int cb = UDPTestServer.parsePacket();
//  if (cb) {
//    UDPTestServer.read(packetBuffer,UDP_PACKET_SIZE);
//    Serial.printf("%s\n",packetBuffer);
//  }
//}
//
//void fncUdpSend()
//{
//  strcpy(udpBuffer,"Zhageshuaiye!");
//  UDPTestServer.beginPacket(ipTarget, 1609);
//  UDPTestServer.printf("%s",udpBuffer);
//  UDPTestServer.endPacket();
//  Serial.println(udpBuffer);
//}
void fncUdpSendint(int i)
{
  Serial.println("Time Sent");
  Serial.println(i);
  udpBuffer[0] = i & 0xff;
  udpBuffer[1] = i >> 8 ;
  udpBuffer[2] = 0;
  UDPTestServer.beginPacket(ipTarget, 1609);
  UDPTestServer.printf("%s",udpBuffer);
  UDPTestServer.endPacket();
  
}
void handleUDPServerint()
{
  cb = UDPTestServer.parsePacket();
//  Serial.println("Im out");
  int i;
//  Serial.println(cb);
  if (cb)
  {
//    Serial.println("I got Here");
    UDPTestServer.read(packetBuffer,UDP_PACKET_SIZE);
    NumberReceived =  packetBuffer[0] | (packetBuffer[1] << 8);
//    i =   packetBuffer[0] | (packetBuffer[1] << 8) ;
//    return i;
  
  }
}

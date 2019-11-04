/*This is the code running on the Receiver Side
The cart is running on station mode, which connects to the access point on the controller
Author: Yupeng Li */

#include <WiFi.h>;//include WiFi library
#include <WiFiUdp.h>;//include UDP packet
int port = 1609;//Port number 1609 used for communication
int cb = 0;//A flag used to tell if a package is received
int val = 0;//A value used to store analogread value
int servoread = 0;
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
Serial.begin(115200);//Setup Serial Communication at 115200(For debug)
Serial.println();   //Printing information regarding to connection
Serial.print("Connecting to");
Serial.println(ssid);
WiFi.mode(WIFI_STA);//Set WiFi mode to station
WiFi.config(myIPaddress,IPAddress(192,168,1,1),IPAddress(255,255,255,0));
//Set up IP address, GateWay and Mask
WiFi.begin(ssid);//Begin WiFi connection at selected WiFi name
WiFi.setSleep(false);//Set mode of WiFi to let it never sleep
while (WiFi.status()!=WL_CONNECTED);//A loop runs until WiFi is connected
{
  delay(500);//wait for 0.5 seconds
  Serial.print(".");//print a dot while waiting
}
Serial.println("WiFi Connected ");//Print WiFi is connected when it is connected

/* The following lines turns the WiFi mode into AP mode */
// WiFi.mode(WIFI_AP);
// WiFi.softAP(ssid);
// delay(100);
// WiFi.softAPConfig(myIPaddress,IPAddress(192,168,1,1),IPAddress(255,255,255,0));

UDPTestServer.begin(port);//begin UDP protocal communication on the assigned port
packetBuffer[UDP_PACKET_SIZE] = 0;//Null terminated the packetBuffer
ledcSetup(LEDC_CHANNEL,LEDC_FREQ_HZ,LEDC_RESOLUTION_BITS);//Set up LEDC channel 0 at 5000HZ, 2^13 resolution
ledcSetup(LEDC_CHANNEL1,LEDC_FREQ_HZ,LEDC_RESOLUTION_BITS);//Set up LEDC Channel 1 at 5000Hz, 2^13 resolution
ledcSetup(LEDC_CHANNEL_SERVO,LEDC_FREQ_HZ_SERVO,LEDC_RESOLUTION_BITS);//Setup LEDC Channel 2 at 50Hz, 2^13 resolution
ledcAttachPin(Enable,LEDC_CHANNEL);//Attach ledc at LEDC_CHANNEL to PWM1
ledcAttachPin(Enable1,LEDC_CHANNEL1);//Attch ledc at LEDC_CHANNEL_1 to PWM2
ledcAttachPin(ServoControl,LEDC_CHANNEL_SERVO);//Attach ledc at LEDC_CHANNEL_SERVO to servo control pin
pinMode(A1,OUTPUT);//Set 1A as output
pinMode(A44,OUTPUT);//Set 4A as output
// pinMode(EnableControl,INPUT);
pinMode(ServoControl,OUTPUT);//Set ServoControl as output

}

void loop() {
  // put your main code here, to run repeatedly:
  receivePacket();//run the receive packet subroutine
  int psudoduty = val -2000;//remap the DC PWM received -> -1000 to 1000
  Serial.println(psudoduty);//Print out the received value (for)
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
    uint32_t servoduty = map(servoread,4000,5000,450,1050)*LEDC_RESOLUTION/10000;
    Serial.println("Servo duty");
    Serial.println(map(val,4000,5000,450,1050));
    ledcWrite(LEDC_CHANNEL_SERVO,servoduty);
  cb = 0;
}

void receivePacket(){
  cb = UDPTestServer.parsePacket();
  if (cb){
    UDPTestServer.read(packetBuffer,UDP_PACKET_SIZE);
    val = (packetBuffer[1]<<8 |packetBuffer[0]);
    servoread = (packetBuffer[3]<<8|packetBuffer[2]);

  }
}

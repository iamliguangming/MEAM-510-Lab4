/*This is the code running on the Receiver Side
The car is running on station mode, which connects to the access point on the controller
Author: Yupeng Li */

#include <WiFi.h>;//include WiFi library
#include <WiFiUdp.h>;//include UDP packet
int port = 1609;//Port number 1609 used for communication
int cb = 0;//A flag used to tell if a package is received
int val = 0;//A value used to store analogread value
int servoread = 0;//A value used to store servo receving value
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
const int A44 =4;//4A used to control the NOT direction on H bridge
const int Enable=22;//Enable pin used to control the PWM on H bridge 1
const int Enable1 = 0;//Enable pin used to control PWM on H bridge 2
const int ServoControl = 33;//Servo control pin used to control the PWM for the servo

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
  Serial.println(psudoduty);//Print out the received value (for debugging)
  uint32_t duty = LEDC_RESOLUTION*abs(psudoduty)/1000;//duty cycle = psudoduty*resolution/1000

//When psudoduty is greater than 0, turn the wheels forward by setting both direction pins to HIGH
  if ((psudoduty) >= 0){
  digitalWrite(A1,HIGH);
  digitalWrite(A44,LOW);
  Serial.println("Rotating forward");
  }

  //When the psudoduty is smaller than 0, turn the wheels backward by setting both direction pins to LOW
  else if ((psudoduty)<0)
  {
  Serial.println("Rotating backward");
  digitalWrite(A1,LOW);
  digitalWrite(A44,HIGH);
  }

  ledcWrite(LEDC_CHANNEL,duty);//Run the motors at given duty cycle to control there speed
  ledcWrite(LEDC_CHANNEL1,duty);//Run the motors at given duty cycle to control there speed
    uint32_t servoduty = map(servoread,4000,5000,450,1050)*LEDC_RESOLUTION/10000;//map the servo duty to 450-1050 which correspond to full left and full right
    Serial.println("Servo duty");
    Serial.println(map(servoread,4000,5000,450,1050));//Print out the servo duty(For debugging)
    ledcWrite(LEDC_CHANNEL_SERVO,servoduty);//PWM to servo motor based on the received duty cycle
  cb = 0;//Set the receive pack back to 0
}


/*Subroutine used to receive and parse packet*/
void receivePacket(){
  cb = UDPTestServer.parsePacket();//Set cb to the length of the packet received
  if (cb){//When cb is not 0
    UDPTestServer.read(packetBuffer,UDP_PACKET_SIZE);//Read packetbuffer with UDP_PACKET_SIZE from the UDP packet received
    val = (packetBuffer[1]<<8 |packetBuffer[0]);//Save the first two bytes of the packet received as the PWM for DC motors
    servoread = (packetBuffer[3]<<8|packetBuffer[2]);//Save the second two bytes of the packet received as the PWM for servo motors

  }
}

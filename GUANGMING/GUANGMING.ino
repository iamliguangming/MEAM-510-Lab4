#define POT 32 // setting up the pot, led, and switch to the GPIO pins that do not get affected in WIFI
#define LED 22
#define SWITCH 33

#include <WiFi.h> //libraries
#include <WiFiUdp.h>

int potread; // initializing the reading from the pot that has been mapped
int cb = 0; // the size of the packet received. 0 if no packet received
int tenth; // converting the pot readings into tenth of seconds to send
int got = 0; // This is the value that I got from the other player
int port = 1609; // my local port
int targetPort = 1609; // other player's port
//both for send and receive
const char* ssid = "NANI"; // my wifi name since I am in AP mode
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
  Serial.println();
  Serial.print("Connecting to ");  Serial.println(ssid);
  pinMode(POT, INPUT); // setting up the pot, leds, and switch
  pinMode(LED, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(SWITCH, INPUT);
//
//  //station mode
//  WiFi.mode(WIFI_STA);
//  WiFi.config(myIPaddress, IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0));
//  WiFi.begin(ssid);

    //AP mode
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid);
    delay(100);
    WiFi.softAPConfig(myIPaddress, IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0));

  udp.begin(port); // starting the udp communications
  UDPTestServer.begin(port);
//
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    Serial.print(".");
//  }
//  Serial.println("WiFi connected");
  packetBuffer[UDP_PACKET_SIZE] = 0; // null terminate

}

void loop() {
  // put your main code here, to run repeatedly:
  potread = map(analogRead(POT), 0, 4095, 100, 10000); // read the pot and map it
  Serial.println(potread);
  digitalWrite(LED, HIGH); // This blinks the LED at 50% duty depending on the pot reading
  for(int i = 0; i<=49; i++){ // This breaks the delay time into smaller parts in case I changed the pot in the middle of a blink
    int potread2 = map(analogRead(POT), 0, 4095, 100, 10000);
    if(abs(potread2 -potread) > 20){ // if I have changed the pot by more than 20, I jump out and use the new pot data.
      break;
    }
    if(digitalRead(SWITCH)){ // if I push the button, I jump out and do the new step
      break;
    }
    delay(potread/100);
  }
  digitalWrite(LED,LOW); // off period of the 50% duty
  for(int i = 0; i<=49; i++){ // same breaking up the delay and interrupts as mentioned above, only difference is that it's for the off cycle
    int potread2 = map(analogRead(POT), 0, 4095, 100, 10000);
    if(abs(potread2 -potread) > 20){
      break;
    }
    if(digitalRead(SWITCH)){
      break;
    }
    delay(potread/100);
  }

  if (digitalRead(SWITCH)) { // if I have pressed the switch
    sendPacket(); // send my time to my opponent
    while(cb == 0){ // wait until I get a message
      receivePacket(); // keep checking if I did get a message
    }
    cb = 0; // reset cb once I do get a message
    int opponent = got; // convert the reveived msg into an integer
//    int lowOppoRange = 0.9*opponent;
//    int highOppoRange = 1.1*opponent+1;
//    int myLow = 0.9*tenth;
//    int myHigh = 1.1*tenth+1;
//    int flag = 0;
//    for( int i = myLow; i<=myHigh ; i++){
//      if( i >= lowOppoRange && i <= highOppoRange){
//        flag = 1;
//      }
//    }
    float opposec = opponent/10; // convert tenths into seconds
    float tenthsec = tenth/10;
    
    float oppofreq = 1/opposec; //convert period into frequency
    float myfreq = 1/tenthsec;
    float high = max(oppofreq, myfreq); // take the higher value
    float low = min(oppofreq, myfreq); // take the lower value
    int hl = 0.9*high; // calc the lower 10% of the higher value
//    int hh = 1.1*high + 1;
    int flag = 0; // initialize flag
    if(low >= hl){ // if the lower value is bigger than the lower 10% of the higher value, then I know it is within 10% of the higher value.
      flag = 1; // set the flag. Reason why I dont need to check if it is less than the higher value or 10% more of the higher value is that we already know low <= high, so there is no way it will be greater than high
    }
    if(flag){ // if the flag is up
      digitalWrite(LED_BUILTIN,HIGH); // I win!
    }// else I lose, which means my light isnt up
    delay(5000); // delay for another game to set up
    digitalWrite(LED_BUILTIN,LOW); // reset game
  }
  receivePacket(); // If I havent pressed a button, I keep checking if my opponent has pressed the button and sent me a message
  if(cb){ // If they did
    sendPacket(); // send them my time so we both can calculate
    cb = 0; // reset the cb
    int opponent = got; // convert the reveived msg into an integer
//    int lowOppoRange = 0.9*opponent;
//    int highOppoRange = 1.1*opponent+1;
//    int myLow = 0.9*tenth;
//    int myHigh = 1.1*tenth+1;
//    int flag = 0;
//    for( int i = myLow; i<=myHigh ; i++){
//      if( i >= lowOppoRange && i <= highOppoRange){
//        flag = 1;
//      }
//    }
    float opposec = opponent/10; // same as above where we find the max and min of the two values, take 10% of the higher value and find the 0.9*max value, which we then see if low >= that
    float tenthsec = tenth/10;
    
    float oppofreq = 1/opposec;
    float myfreq = 1/tenthsec;
    float high = max(oppofreq, myfreq); // take the higher value
    float low = min(oppofreq, myfreq); // take the lower value
    int hl = 0.9*high;
//    int hh = 1.1*high + 1;
    int flag = 0;
    if(low >= hl){
      flag = 1;
    }
    if(flag == 0){ // if my opponent did not guess correctly
      digitalWrite(LED_BUILTIN,HIGH); // I win!
    }// or I lose if flag == 1, then led wont turn on
    delay(5000); // delay for the next game
    digitalWrite(LED_BUILTIN,LOW); // reset game
  }
  delay(1);
}

void receivePacket() { // FOR RECEIVING
  cb = UDPTestServer.parsePacket(); // parse packet. Sees if I get something
  if(cb){ // If I did
    UDPTestServer.read(packetBuffer, UDP_PACKET_SIZE); // read the packet, though it is in bytes
//    Serial.printf("%s\n", packetBuffer);
//    int first = (int)(packetBuffer[0]);
//    int second = (int)(packetBuffer[1]<<8);
    got = (packetBuffer[1]<<8) | packetBuffer[0]; // convert it to an integer
    Serial.println(got);
//    cb = 0;
  }
}

void sendPacket() {
  Serial.println("im in");
  
  tenth = potread / 100; // change the pot readings into tenths (1 = 0.1 seconds, 100 = 10 seconds)
  Serial.println(tenth);
//  char buf[8];
//  itoa(tenth, buf, 10);
//  strcpy(udpBuffer, buf);// send what ever you want upto buffer size
  udpBuffer[0] = tenth & 0xff; // send LSB
  udpBuffer[1] = tenth >> 8; // send MSB
  udpBuffer[2] = 0; // null terminate
  Serial.println(tenth);
  Serial.println(udpBuffer[0]);
  
  Serial.println("printed");
  udp.beginPacket(ipTarget, targetPort);  // send to opponent port
  udp.printf("%s", udpBuffer);
  udp.endPacket(); // end msg
  delay(100); // used to prevent multiple msgs from a long press of a button.
}

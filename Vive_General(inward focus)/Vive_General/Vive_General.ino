 #define diode 33// output from the circuit
#define diode2 34    //

#define autoMode 4
#define statePin1 19
#define statePin2  18

//#define pulse 22
int prevT = 0; // the previous time for calculating both the pulse width and the time for x and y distance
int currT = 0; // find the current time using micros()
int flagx = 1; // flag for the x pulse. Returns 0 if we looked at the x pulse and sets y flag = 1
int flagy = 0; // flag for the y pulse. Returns 0 if we looked at the y pulse. Does not reset the x flag
int start = 0; // remembers the rising edge of the synce pulse to calculate the x/y time distance
int counter = 0; // counts the sync pulses. If counter = 3, then we know the next pulse is an x pulse.
int timediff = 0; // calculates the time difference between two times, whether for pulse width or x/y distance

int prevT2 = 0; // the previous time for calculating both the pulse width and the time for x and y distance
int currT2 = 0; // find the current time using micros()
int flagx2 = 1; // flag for the x pulse. Returns 0 if we looked at the x pulse and sets y flag = 1
int flagy2 = 0; // flag for the y pulse. Returns 0 if we looked at the y pulse. Does not reset the x flag
int start2 = 0; // remembers the rising edge of the synce pulse to calculate the x/y time distance
int counter2 = 0; // counts the sync pulses. If counter = 3, then we know the next pulse is an x pulse.
int timediff2 = 0; // calculates the time difference between two times, whether for pulse width or x/y distance

float DirectionX;
float DirectionY;
float NormalX;
float NormalY;

int autoModeFlag = 1;
int flagYback = 1;

int xFront = 0;
int yFront = 0;
int xBack =0;
int yBack = 0;

int steps = 1;
int turned = 0;

void GoStraight();
void clearSyncPulse(int SignalCleared);
void TurnRight();
void TurnLeft();
void StopIt();

void IRAM_ATTR calcT(){ // interrupt function
  if(digitalRead(diode) == LOW){ // This checks for pulse width
    currT = micros(); // finds time in microseconds
    timediff = currT - prevT; // calculate time of the pulse width
    if(timediff < 60){ // if the width is less than 60, which is less than the pulse width of the sync pulse, but larger than the x/y pulses
      if(flagx){ // if the x flag is on
        // Serial.print("x-pulse: "); // print out the pulse value
        // Serial.println(timediff);
        delayMicroseconds(1);
        flagx = 0; // set the flag to 0 so that we dont read the next pulse as x
        flagy = 1; // turn this flag on to read the next pulse as y
      }
      else if(flagy){ // if x flag isnt on but y flag is on
        // Serial.print("  y-pulse: "); // print out the pulse value
        // Serial.println(timediff);
        delayMicroseconds(1);
//        flagx = 1;
        flagy = 0; // reset the flag back to 0. At this point, both the x and y flags are off.
      }
      counter = 0; // reset the counter. This is mainly for the else statement where we look at the counter value and the flags on line 49 (if nothing changes)
    }
    else{
//      Serial.println("im counting up");
      counter++; // increments the counter
//      Serial.println(counter);
      if(counter == 3){ // if I have counted 3 sync pulses, then I know the next pulse has to be an x pulse.
        flagx = 1; // because we know the next one is an x pulse, we can safely set the x flag on and prepare for the next pulse.
        counter = 1; // reset the counter to 1. This time we did not set it to 0 because we need to calculate the x time distance, which requires the counter to be 1 (as seen in the else statement below)
//        Serial.println(counter);
      }
    }
    start = prevT; // sets the start time to the rising edge of the pulse. This helps find the x/y time distance
    prevT = currT; // resets the previous time to the current time to be used in finding the pulse width
  }
  else{ // Here we are looking at the rising edge.
//    Serial.println("im in else");
    currT = micros(); // finds the time in microseconds
    timediff = currT - start; //calculate the time difference
    if(counter == 1 && (flagx || flagy)){ // checks whether the counter is 1 and if either flag is on. The counter is always on before the x and y pulses, as well as after the y pulse; however, we prevent the issue of counting the sync pulse after the y pulse by checking the flags, which are both 0 after the y pulse.
      if(flagx){ // checks the x flag. If x flag is on here, then we are on the x pulse. Else, on the y pulse.
        // Serial.print("    x time:"); // print the x time distance
        // Serial.println(timediff);
        if(timediff >=1000 && timediff <=8000){
          xFront = timediff;
        }
        else{
          xFront = 0;
        }
        delayMicroseconds(1);
      }
      else{ // on y pulse
        // Serial.print("      y time:"); // print the y time distance
        // Serial.println(timediff);
        if(timediff >=1000 && timediff <=8000){
          yFront = timediff;
        }
        else{
          yFront = 0;
        }
        delayMicroseconds(1);
      }
    }
    prevT = currT; // set the previous time to current time (on rising edge) for pulse width.
  }
}
void IRAM_ATTR calcT2(){ // interrupt function
  if(digitalRead(diode2) == LOW){ // This checks for pulse width
    currT2 = micros(); // finds time in microseconds
    timediff2 = currT2 - prevT2; // calculate time of the pulse width
    if(timediff2 < 60){ // if the width is less than 60, which is less than the pulse width of the sync pulse, but larger than the x/y pulses
      if(flagx2){ // if the x flag is on
        // Serial.print("x2-pulse: "); // print out the pulse value
        // Serial.println(timediff2);
        delayMicroseconds(1);
        flagx2= 0; // set the flag to 0 so that we dont read the next pulse as x
        flagy2 = 1; // turn this flag on to read the next pulse as y
      }
      else if(flagy2){ // if x flag isnt on but y flag is on
        // Serial.print("  y2-pulse: "); // print out the pulse value
        // Serial.println(timediff2);
        delayMicroseconds(1);
//        flagx2 = 1;
        flagy2 = 0; // reset the flag back to 0. At this point, both the x and y flags are off.
      }
      counter2 = 0; // reset the counter. This is mainly for the else statement where we look at the counter value and the flags on line 49 (if nothing changes)
    }
    else{
//      Serial.println("im counting up");
      counter2++; // increments the counter
//      Serial.println(counter2);
      if(counter2 == 3){ // if I have counted 3 sync pulses, then I know the next pulse has to be an x pulse.
        flagx2 = 1; // because we know the next one is an x pulse, we can safely set the x flag on and prepare for the next pulse.
        counter2 = 1; // reset the counter to 1. This time we did not set it to 0 because we need to calculate the x time distance, which requires the counter to be 1 (as seen in the else statement below)
//        Serial.println(counter);
      }
    }
    start2 = prevT2; // sets the start time to the rising edge of the pulse. This helps find the x/y time distance
    prevT2 = currT2; // resets the previous time to the current time to be used in finding the pulse width
  }
  else{ // Here we are looking at the rising edge.
//    Serial.println("im in else");
    currT2 = micros(); // finds the time in microseconds
    timediff2 = currT2 - start2; //calculate the time difference
    if(counter2 == 1 && (flagx2 || flagy2)){ // checks whether the counter is 1 and if either flag is on. The counter is always on before the x and y pulses, as well as after the y pulse; however, we prevent the issue of counting the sync pulse after the y pulse by checking the flags, which are both 0 after the y pulse.
      if(flagx2){ // checks the x flag. If x flag is on here, then we are on the x pulse. Else, on the y pulse.
        // Serial.print("    x2 time:"); // print the x time distance
        // Serial.println(timediff2);
        if(timediff2 >=1000 && timediff2 <=8000){
          xBack = timediff2;
        }
        else{
          xBack = 0;
        }
        delayMicroseconds(1);
      }
      else{ // on y pulse
        // Serial.print("      y2 time:"); // print the y time distance
        // Serial.println(timediff2);
        if(timediff2 >=1000 && timediff2 <=8000){
          yBack = timediff2;
        }
        else{
          yBack = 0;
        }
        delayMicroseconds(1);
      }
    }
    prevT2 = currT2; // set the previous time to current time (on rising edge) for pulse width.
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); // initialize serial
  pinMode(diode,INPUT);
  pinMode(diode2,INPUT);
  pinMode(autoMode,INPUT);
  pinMode(statePin1,OUTPUT);
  pinMode(statePin2,OUTPUT);
//  pinMode(pulse, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(diode), calcT, CHANGE); // make the interrupt
  attachInterrupt(digitalPinToInterrupt(diode2),calcT2, CHANGE);

  int turned = 0;
}

int countup = 1;
int xfinal = 3205;
int yfinal = 3537;
int flagTime = 1;
int initialYPos = 0;
int flagLR = 0;
int flagCheckLR = 1;

void loop() {
  // put your main code here, to run repeatedly:

  if (xFront > 8000)
  {
    xFront = 0;
  }
 
 if (yFront > 8000)
  {
    yFront = 0;
  }  
   if (xBack > 8000)
  {
    xBack = 0;
  }
   if (yBack > 8000)
  {
    yBack= 0;
  }

if ((xFront>=1000) && (yFront>=1000) && (xBack>=1000) && (yBack>=1000))
{
  Serial.print("xFront: ");
  Serial.println(xFront);
  Serial.print("yFront: ");
  Serial.println(yFront);
  Serial.print("xBack: ");
  Serial.println(xBack);
  Serial.print("yBack: ");
  Serial.println(yBack);
  DirectionX = xFront - xBack;
  Serial.println(DirectionX);
  Serial.println(DirectionX * DirectionX);
  DirectionY = yFront - yBack;
  Serial.println(DirectionY);
  Serial.println(DirectionY * DirectionY);
  float NormalX = DirectionX / sqrt((DirectionX * DirectionX) + (DirectionY * DirectionY));
  float NormalY = DirectionY / sqrt((DirectionX * DirectionX) + (DirectionY * DirectionY));
  Serial.print("NormalX");
  Serial.println(NormalX);
  Serial.print("NormalY");
  Serial.println(NormalY);
//  GoStraight();

  float err = 0.02;
//  flagYback = 1;
//  Serial.println(flagYback);
//  if(flagYback == 1){ 
//    Serial.println("        I'm in flag");
//    if(yBack>=1000){
//      initialYPos = yBack; 
//      Serial.println(initialYPos);
//      flagYback = 0; 
//    }      
//  }
  int bufferDistanceL = 450; // replace this with the distance you want to be from the final position. This is used to keep the radius of curvature in mind.
  int bufferDistanceR = 450;
  int initialYPosR = 2041;
  int initialYPosL = 6045;
  int wallDistL = 200;

  if(flagCheckLR){
    if(yFront>=yfinal){
      flagLR = 1; // Left
    }
    else{
      flagLR = 0; // Right
    }
    flagCheckLR = 0;
  }

  if(flagLR){ // im on the left side
//    if(NormalX != 0 && NormalY != 0){
    Serial.println("          I'm in left side");
    if(turned == 0){
      Serial.println("                  I'm in 0");
      if(1+NormalX >= err){ // until I face North
        TurnLeft();
      }
      else{
        Serial.println("        I turned");
        StopIt();
        turned = 1;
      }
    }
    else if(turned == 1){
      Serial.println(initialYPos);
      if(1-NormalY >= err && ((initialYPosL - yFront ) >= wallDistL)){ // insert distance from "wall" here // May need a smaller than 1 foot buffer here.
        Serial.println("                  I'm in 1!!!!!!!");
        TurnLeft();
      }
      else{
        Serial.println("        I turned again");
        StopIt();
//        if(initialYPos >= 1000){
          turned = 2;
//        }
      }
    }
      else if(turned == 2){
        Serial.println("                  I'm in 2");
        if(1 + NormalX >= 0.005){
          TurnRight();
        }
        else{
          StopIt();
          turned = 3;
        }
      }
    else if(turned == 3){
//      if(abs(xfinal - xFront) >= bufferDistanceL + 120){ // input around 1 foot here. This is the radius of curvature
      Serial.println("                  I'm in 3");
      if(abs(xfinal - xFront) >= bufferDistanceL+200){
        GoStraight();
      }
      else{
        StopIt();
        turned = 4;
      }
    }
    else if(turned == 4){
      Serial.println("                  I'm in 4");
      if(NormalY + 1.00 >= 0.25*err){
//      if((NormalX <=0.05){
        TurnRight();
      }
      else{
        StopIt();
        turned = 5;
      }
    }
    else if(turned == 5){
      Serial.println("                  I'm in 5");
      if(abs(yFront - yfinal) >= bufferDistanceL-280){// may need a different buffer Distance !!!!!!!!!!!!!!!
        GoStraight();
      }
      else{
        StopIt();
        turned = 6;
      }
    }
    else if(turned == 6){
      Serial.println("                  I'm in 6");
      if(1+NormalX >= err){
          TurnLeft();
      }
      else{
        StopIt();
        turned = 7;
      }
    }
    else{
      Serial.println("                  I'm in 7");
      GoStraight();
    }
//  }
  }
  else{
    int xfinal = 3211;
    int yfinal = 4302; // temp position of the blue button
//    if(NormalX != 1.00 && NormalY != 1.00){
    if(abs(yFront - initialYPosR) <= wallDistL && turned != 7 && 1+NormalX>= err){
      TurnLeft();
      turned = 3;
    } 
    else if(abs(xFront - xfinal) <= 150 && turned != 7 && NormalX <= 0.05){
      TurnLeft();
      turned = 5;
    }
    else{
    if(turned == 0){
      Serial.println("          I'm in right side");
      Serial.println("                  I'm in 0");
      if(1+NormalX >= err && NormalY <=0){ // until I face North
        TurnRight();
      }
      else if(1+NormalX >= err && NormalY >=0){ // until I face North
        TurnRight();
      }
      else{
        Serial.println("        I turned");
        StopIt();
        turned = 1;
      }
    }
    else if(turned == 1){
      Serial.println(initialYPos);
      if(1+NormalY >= err && ((yFront - initialYPosR) >= wallDistL)){ // insert distance from "wall" here // May need a smaller than 1 foot buffer here.
        Serial.println("                  I'm in 1!!!!!!!");
        TurnRight();
      }
      else{
        Serial.println("        I turned again");
        StopIt();
//        if(initialYPos >= 1000){
          turned = 2;
//        }
      }
    }
      else if(turned == 2){
        Serial.println("                  I'm in 2");
        if(1 + NormalX >= 0.005){
          TurnLeft();
        }
        else{
          StopIt();
          turned = 3;
        }
      }
    else if(turned == 3){
      Serial.println("                  I'm in 3");
      if(abs(xfinal - xFront) >= bufferDistanceL){ // input around 1 foot here. This is the radius of curvature
        GoStraight();
      }
      else{
        StopIt();
        turned = 4;
      }
    }
    else if(turned == 4){
      Serial.println("                  I'm in 4");
//      if(1 - NormalY >= 0.25*err){
      if(NormalX <=0.05){
        TurnLeft();
      }
      else{
        StopIt();
        turned = 5;
      }
    }
    else if(turned == 5){
      Serial.println("                  I'm in 5");
      if(abs(yFront - yfinal) >= bufferDistanceL-280){// may need a different buffer Distance !!!!!!!!!!!!!!!
        GoStraight();
      }
      else{
        StopIt();
        turned = 6;
      }
    }
    else if(turned == 6){
      Serial.println("                  I'm in 6");
      if(1+NormalX >= err){
          TurnRight();
          Serial.println("Im turning Right");
      }
      else{
        StopIt();
        turned = 7;
      }
    }
    else{
      Serial.println("                  I'm in 7");
      GoStraight();
    }
  }
  }
//  }
}
}                                                                                                                                                                   

//void clearSyncPulse(int SignalCleared)
//{
//  Serial.println("Im in");
//  Serial.print(countup);
//  Serial.print(" :");
//  Serial.println(SignalCleared);
//  if (SignalCleared > 8000)
//  {
//    SignalCleared = 0;
//  }
//  Serial.println("Im Out");
//
//}
// void GoStraight(int Xway, int Yway, float NormalX, float NormalY)
// {
//   digitalWrite(A1,HIGH);
//   digitalWrite(N_A1,LOW);
//   ledcWrite(LEDC_CHANNEL,fullduty);
//    if (Xway == 1 && Yway == 0 )
//    {
//     ServoAngleDuty = map((NormalY-Yway)*1000,-1000,1000,450*LEDC_RESOLUTION/10000,1050*LEDC_RESOLUTION/10000);
//    }
//    else if (Xway == -1 && Yway == 0)
//   {
//     ServoAngleDuty = map ((NormalY - Yway)*1000,1000,-1000,450*LEDC_RESOLUTION/10000,1050*LEDC_RESOLUTION/10000);
//   }
//   else if (Xway == 0 && Yway == 1 )
//   {
//     ServoAngleDuty = map((NormalX - Xway) * 1000,-1000,1000, 450*LEDC_RESOLUTION/10000,1050*LEDC_RESOLUTION/10000);
//   }
//   else if (Xway == 0 && Yway == -1)
//   {
//     ServoAngleDuty = map((NormalX - Xway) * 1000, 1000,-1000, 450*LEDC_RESOLUTION/10000,1050*LEDC_RESOLUTION/10000);
//   }
//   ledcWrite(LEDC_CHANNEL_SERVO,ServoAngleDuty);
// }

void TurnLeft()
{
  digitalWrite(statePin1,HIGH);
  digitalWrite(statePin2,LOW);
}

void TurnRight()
{
  digitalWrite(statePin1,LOW);
  digitalWrite(statePin2,HIGH);
}

void StopIt()
{
  digitalWrite(statePin1,LOW);
  digitalWrite(statePin2,LOW);
}

void GoStraight()
{
  digitalWrite(statePin1,HIGH);
  digitalWrite(statePin2,HIGH);
}

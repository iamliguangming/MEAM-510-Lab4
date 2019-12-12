#define diode 33 // input to the circuit for the photodiode
#define diode2 34    //input to the circuit for the photodiode

#define autoMode 4 // input from main ESP to know when it is autoMode
#define statePin1 19 // output to the main ESP to tell it how to move
#define statePin2  18 //output to the main ESP to tell it how to move

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

float DirectionX; // takes the difference of the x positions of the 2 Vive signals and finds the x component of the vector it is pointing to
float DirectionY; // take the difference of the y positions of the Vive signals and finds the y component of the vector
float NormalX; // Normalizes the Direction X to  be a value from 0-1
float NormalY; // Normalizes the DIrectionY to be a value from 0-1

int xFront = 0; // The front Vive's x position
int yFront = 0; // Front vive's y position
int xBack =0; // back vive's x position
int yBack = 0; // back vive's y position

int turned = 0; // a counter that tells the autonomous mode what step it is on/ should go to

void GoStraight(); // tells the main ESP to tell the robot to go straight
void TurnRight(); // tells the car to turn right
void TurnLeft(); // tells the car to turn left
void StopIt(); // stops the car

void IRAM_ATTR calcT(){ // interrupt function for Vive
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
        if(timediff >=1000 && timediff <=8000){ // checks if the signal received is a value from 1000-8000. Any value smaller that 1000 is a bad value, and any value greater that 8000 means the Vive picked up a sync to sync pulse signal, which means the signal was blocked
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
        if(timediff >=1000 && timediff <=8000){ // checks if the signal received is a value from 1000-8000. Any value smaller that 1000 is a bad value, and any value greater that 8000 means the Vive picked up a sync to sync pulse signal, which means the signal was blocked
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
        if(timediff2 >=1000 && timediff2 <=8000){ // checks if the signal received is a value from 1000-8000. Any value smaller that 1000 is a bad value, and any value greater that 8000 means the Vive picked up a sync to sync pulse signal, which means the signal was blocked
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
        if(timediff2 >=1000 && timediff2 <=8000){ // checks if the signal received is a value from 1000-8000. Any value smaller that 1000 is a bad value, and any value greater that 8000 means the Vive picked up a sync to sync pulse signal, which means the signal was blocked
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
  pinMode(diode,INPUT); // sets up the pinModes
  pinMode(diode2,INPUT);
  pinMode(autoMode,INPUT);
  pinMode(statePin1,OUTPUT);
  pinMode(statePin2,OUTPUT);
//  pinMode(pulse, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(diode), calcT, CHANGE); // make the interrupt
  attachInterrupt(digitalPinToInterrupt(diode2),calcT2, CHANGE);

  int turned = 0; // reset the counter value to 0 everytime it sets up
}

int xfinal = 3205; // If starting on the left side, this was the final position we wanted to go to to hit the button
int yfinal = 3537; // the y position of the button
int flagLR = 0; // a one-time flag that sees if I am on the left or right side of the final position. This was added since it was possible to go past the final position on accident, which caused the code to switch over and start running the other side's code. (ex. We are supposed to turn right and hit the button, but since it went past the final position, it turned left instead, which was what it was supposed to do if we started on the other side)
int flagCheckLR = 1; // Same as above. This tells me that I only want this flag to be triggered once. Once it is set to 0, it will never get set to 1 unless restarted.

void loop() {
  // put your main code here, to run repeatedly:

  if (xFront > 8000) // checking again if we had a sync to sync pulse
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

if ((xFront>=1000) && (yFront>=1000) && (xBack>=1000) && (yBack>=1000)) // checking if we had bad data
{
  Serial.print("xFront: "); // print out statements for debugging
  Serial.println(xFront);
  Serial.print("yFront: ");
  Serial.println(yFront);
  Serial.print("xBack: ");
  Serial.println(xBack);
  Serial.print("yBack: ");
  Serial.println(yBack);
  DirectionX = xFront - xBack; // finds the difference in x position between the 2 vives
  Serial.println(DirectionX);
  Serial.println(DirectionX * DirectionX);
  DirectionY = yFront - yBack; // finds the difference in the y positions between the 2 vives
  Serial.println(DirectionY);
  Serial.println(DirectionY * DirectionY);
  float NormalX = DirectionX / sqrt((DirectionX * DirectionX) + (DirectionY * DirectionY)); // Normalizes them to be a value from 0-1
  float NormalY = DirectionY / sqrt((DirectionX * DirectionX) + (DirectionY * DirectionY)); // Normalizes them to be a value from 0-1
  Serial.print("NormalX");
  Serial.println(NormalX);
  Serial.print("NormalY");
  Serial.println(NormalY);
//  GoStraight();

  float err = 0.02; // set a leeway for the car to turn so that it has some room for error when driving

  int bufferDistanceL = 450; // this is the value we have for making sure we have enough room to turn. This value is updated in the if statements below since the field and Vive placement was shit, so tuning had to be done
  int bufferDistanceR = 450; // same as above, but for the right side
  int initialYPosR = 2041; // This is for the side walls. We used this number for making sure we dont get too close to it
  int initialYPosL = 6045; // same as above
  int wallDistL = 200; // this is the distance we wanted to keep from the walls so that we could turn and not bump into the wall.

  if(flagCheckLR){ // checks if we are on the left side of right side of our final destination
    if(yFront>=yfinal){ // if my vive sensor is on the left of the final position
      flagLR = 1; // Left
    }
    else{
      flagLR = 0; // Right
    }
    flagCheckLR = 0; // set to 0 and never reset it. This locks in our position of being on one side
  }

  if(flagLR){ // im on the left side. HOWEVER, THE LEFT SIDE CODE DOES NOT WORK PROPERLY AS WE ONLY HAD TIME TO CONFIGURE FOR THE RIGHT SIDE. PLEASE SKIP TO THE RIGHT SIDE CODE
//    if(NormalX != 0 && NormalY != 0){
    Serial.println("          I'm in left side");
    if(turned == 0){ // first step
      Serial.println("                  I'm in 0");
      if(1+NormalX >= err){ // until I face North
        TurnLeft();
      }
      else{
        Serial.println("        I turned");
        StopIt(); // this command is helpful when breaking the code in chunks. Without this command, the program will keep turning even though the value of turned incremented
        turned = 1;
      }
    }
    else if(turned == 1){ // 2nd step
      Serial.println(initialYPos);
      if(1-NormalY >= err && ((initialYPosL - yFront ) >= wallDistL)){ // insert distance from "wall" here // May need a smaller than 1 foot buffer here.
        Serial.println("                  I'm in 1!!!!!!!");
        TurnLeft();
      }
      else{
        Serial.println("        I turned again");
        StopIt();
        turned = 2;
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
  else{ // THIS IS THE RIGHT SIDE CODE, WHERE MOST OF THE COMMENTS WILL BE PLACED AND HAVE IMPORTANCE.
    int xfinal = 3211; // x position of the blue button
    int yfinal = 4302; // y position of the blue button
//    if(NormalX != 1.00 && NormalY != 1.00){
    if(abs(yFront - initialYPosR) <= wallDistL && turned != 7 && 1+NormalX>= err){ // this is for the side wall. Supposed to help steer us away from the walls in auto mode.
      TurnLeft();
      turned = 3; // skip to the "go straight" part of the code. Reason why we did not go to turned = 2 is because sometimes the car would turn too far, making it keep turning left forever
    } 
    else if(abs(xFront - xfinal) <= 150 && turned != 7 && NormalX <= 0.05){ // this is for the ramp wall, making sure we dont run into it until the last step when we want to hit the button
      TurnLeft();
      turned = 5; // skip to the "go straight" part of the code so we dont keep turning left.
    }
    else{
    if(turned == 0){ //step-by-step process
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
      if(1+NormalY >= err && ((yFront - initialYPosR) >= wallDistL)){ //This is to keep turning right until I am facing right OR I am close enough to the wall where I need to make a left turn now to face north 
        Serial.println("                  I'm in 1!!!!!!!");
        TurnRight();
      }
      else{
        Serial.println("        I turned again");
        StopIt();
        turned = 2;
      }
    }
      else if(turned == 2){
        Serial.println("                  I'm in 2");
        if(1 + NormalX >= 0.005){ // Here, I want to face North
          TurnLeft();
        }
        else{
          StopIt();
          turned = 3;
        }
      }
    else if(turned == 3){
      Serial.println("                  I'm in 3");
      if(abs(xfinal - xFront) >= bufferDistanceL){ // I need to keep going straight until I reach a certain distance so that I can make a turn
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
      if(NormalX <=0.05){ // I want to keep turning, but dont stop until I reach a bit further than parallel to the x axis. The reason is because the car leans to the right, so we need to make the angle to the left so that it goes "straight" to the button
        TurnLeft();
      }
      else{
        StopIt();
        turned = 5;
      }
    }
    else if(turned == 5){
      Serial.println("                  I'm in 5");
      if(abs(yFront - yfinal) >= bufferDistanceL-280){// Keep straight until I am almost near the button
        GoStraight();
      }
      else{
        StopIt();
        turned = 6;
      }
    }
    else if(turned == 6){
      Serial.println("                  I'm in 6");
      if(1+NormalX >= err){ // turn into the button
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
      GoStraight(); // keep going straight into the button. The reason why I did not kill the motors was that if there was someone who came by and pushed us, it would be harder for them to do so
    }
  }
  }
//  }
}
}                                                                                                                                                                   


// Below are the different ways we were able to tell the main ESP how we wanted the servo and motors to work
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

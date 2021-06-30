#include <SoftwareSerial.h>
//blue tooth pins
SoftwareSerial mySerial(9,10);//rx,tx
//flex sensor variables
const int FLEX_PIN = A0; // Pin connected to each flex sensor's voltage divider
const int FLEX_PIN2 = A1;
const int FLEX_PIN3 = A2;
const int FLEX_PIN4 = A3;
const int FLEX_PIN5 = A4;

const float VCC = 5; 
const float R_DIV = 12000.0; //Resistor used for index finger and thumb
const float R_DIV2 = 1500.0; //Resistor used for remainder of fingers
// Upload the code, then try to adjust these values to more
// accurately calculate bend degree.
const float STRAIGHT_RESISTANCE = 12000.0; // resistance when straight
const float BEND_RESISTANCE = 21000.0; // resistance at 90 deg
const float STRAIGHT_RESISTANCE2 = 9450.0; // resistance when straight
const float BEND_RESISTANCE2 = 22500.0; // resistance at 90 deg
const float STRAIGHT_RESISTANCE3 = 13400.0; // resistance when straight
const float BEND_RESISTANCE3 = 31000.0; // resistance at 90 deg
const float STRAIGHT_RESISTANCE4 = 12000.0; // resistance when straight
const float BEND_RESISTANCE4 = 21000.0; // resistance at 90 deg
const float STRAIGHT_RESISTANCE5 = 30000.0;//resistance when straight
const float BEND_RESISTANCE5 = 39000.0;//resistance at 90 deg

//first argument correlates to fingers
//index = 1, ring = 2 ... thumb = 5
//second argument is debug which will print to console
float calcFlex(int i, bool d){
  int flexADC;
  float SR, BR;
  float RD;
  switch(i){
    case 1:
      flexADC = analogRead(FLEX_PIN);
      SR = STRAIGHT_RESISTANCE;
      BR = BEND_RESISTANCE;
      RD = R_DIV;
      if(d){
      //Serial.println("Index Finger:");
      }
      break;
    case 2:
      flexADC = analogRead(FLEX_PIN2);
      SR = STRAIGHT_RESISTANCE2;
      BR = BEND_RESISTANCE2; 
      RD = R_DIV2;     
      if(d){
      //Serial.println("Middle Finger:");
      }
      break;
    case 3:
      flexADC = analogRead(FLEX_PIN3);
      SR = STRAIGHT_RESISTANCE3;
      BR = BEND_RESISTANCE3; 
      RD = R_DIV2;     
      if(d){
      //Serial.println("Ring Finger:");
      }
      break;
    case 4:
      flexADC = analogRead(FLEX_PIN4);
      SR = STRAIGHT_RESISTANCE4;
      BR = BEND_RESISTANCE4; 
      RD = R_DIV2;     
      if(d){
      //Serial.println("Pinky Finger:");
      }
      break;
   case 5:
      flexADC = analogRead(FLEX_PIN5);
      SR = STRAIGHT_RESISTANCE5;
      BR = BEND_RESISTANCE5; 
      RD = R_DIV;     
      if(d){
      //Serial.println("Thumb:");
      }
      break;
  }

      float flexV = flexADC * VCC / 1023.0;
      float flexR = RD * (VCC / flexV - 1.0);
      float angle = map(flexR, SR, BR,0, 90.0);
      if(angle >=180){
        angle = 179;
      }
      if(angle <= 0){
        angle = 0;
      }
      //now we need to encode the values
      //pointer: 0-179 middle:180-359 ring:360 - 539 pinky: 540-719 thumb: 720-899
      switch(i){
        case 1:
          angle = angle;
          break;
        case 2:
          angle = angle + 180;
          break;
        case 3:
          angle = angle + 360;
          break;
        case 4:
          angle = angle + 540;
          break;
        case 5:
          angle = angle + 720;
          break;
      }
     // if(d){
     // Serial.println("Resistance: " + String(flexR) + " ohms");
     // Serial.println("flexADC: " + String(flexADC));
      
     // Serial.println("Bend: " + String(angle) + " degrees");
     // Serial.println();
     // }
      
      return angle;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial){
    ;
  }
  Serial.println("Code Started");
  //set data rate for the software serial port
  mySerial.begin(9600);
  pinMode(FLEX_PIN, INPUT);
  pinMode(FLEX_PIN2, INPUT);
  pinMode(FLEX_PIN3, INPUT);
  pinMode(FLEX_PIN4, INPUT);
  pinMode(FLEX_PIN5, INPUT);
}
void sendValue(int pos){
   //converting inputted int into a string
   String temp1 = String(pos);
   temp1+='\n';
   int temp1_len = temp1.length()+1;
   char char_array[temp1_len];
   temp1.toCharArray(char_array,temp1_len);

   //iterating through to send the values
   for(int i = 0; i<temp1_len ; i++){
      mySerial.write(char_array[i]);
      //after sending the \n we are done
      if(char_array[i] == '\n'){
        break;
      }
   }
   Serial.println("Sent: " + String(char_array));
}
int counter = 1;
void loop() {
  // put your main code here, to run repeatedly:
  //testing for pointer finger
  if(counter == 6){
    counter = 1;
  }
  int khoi = (calcFlex(counter,true));
  sendValue(khoi);
  counter++;
  /*String test = "Emily Hoang is cool";
  test+='\n';
  int char_len = test.length();
  char char_array[char_len];
  char send_array[char_len];
  test.toCharArray(char_array,char_len);
  //iterate through to send
  for(int i = 0; i<char_len; i++){
     mySerial.write(char_array[i]);
     //Serial.println("Sent emily");
      //delay(50);
  }
  Serial.println("Sent emily");*/
  delay(500);
  //mySerial.write(test);
  
}

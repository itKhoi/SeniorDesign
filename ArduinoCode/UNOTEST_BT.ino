#include <SoftwareSerial.h>
#include <Servo.h>
SoftwareSerial mySerial(11,12);//rx,tx
int counter = 0;
//servo setup for robotic hand
int servoPinThumb = 3;
Servo Servo5;
int servoPinPoint = 5;
Servo Servo1;
int servoPinMiddle = 6;
Servo Servo2;
int servoPinRing = 9;
Servo Servo3;
int servoPinPink = 10;
Servo Servo4;

void setup() {
  // put your setup code here, to run once:
  //Open Serial communications and wait for port to open
  Serial.begin(9600);
  while(!Serial){
    ;
  }
  Serial.println("Code Starting");
  //set the data rate for the softwareserial port
  mySerial.begin(9600);
  Servo1.write(0);
  Servo2.write(0);
  Servo3.write(0);
  Servo4.write(0);
  Servo5.write(0);
  Servo1.attach(servoPinPoint);
  Servo2.attach(servoPinMiddle);
  Servo3.attach(servoPinRing);
  Servo4.attach(servoPinPink);
  Servo5.attach(servoPinThumb);
}

boolean newData = false;
void processData(){
  int x = 0;
    char receivedChars[32];
    newData = false;
    while(mySerial.available()>0 && newData == false){
      char rc = mySerial.read();
      if(rc == '\n'){
        receivedChars[x] = '\0';
        newData = true;
      }
      else{
        receivedChars[x] = rc;
        x++;
        
      }
    }
    int value = atoi(receivedChars);
    //now need to see which finger it is from this
    if(value<180){
      Serial.println("Index Finger Received: " + String(value));
      motorControl(value , 1);
    }
    else if(value <360){
      value = value - 180;
      Serial.println("Middle Finger Received: " + String(value));
      motorControl(value,2);
    }
    else if(value < 540){
      value = value - 360;
      Serial.println("Ring Finger Received: " + String(value));
      motorControl(value,3);
    }
    else if(value < 720){
      value = value - 540;
      Serial.println("Pinky Finger Received: " + String(value));
      motorControl(value,4);
    }
    //if its higher than 720 its the thumb
    else{
      value = value - 720;
      Serial.println("Thumb Received: " + String(value));
      motorControl(value,5);
    }
    //Serial.println("Recieved: " + String(value));
    delay(500);
}
//want to know previous location because if change is less than 15 degrees maybe no point in moving
int curPos1 = 0;
int curPos2 = 0;
int curPos3 = 0;
int curPos4 = 0;
int curPos5 = 0;
void motorControl(int pos, int finger){
  //1: index 2: middle 3: ring 4: pinky 5: thumb
  //if its 0-20 or 160-180 just do all the way
  boolean marker = true;
  if(pos>=160){
    pos = 170;
  }
  else if(pos<=20){
    pos = 0;
  }
  switch(finger){
    case 1:
      if(abs(pos - curPos1) <15){break;}
      Servo1.write(pos);
      curPos1 = pos;
      Serial.println("Pointer finger moved");
      break;
    case 2:
      if(abs(pos - curPos2) <15){break;}
      Servo2.write(pos);
      curPos2 = pos;
      Serial.println("Middle finger moved");
      break;
    case 3:
      if(abs(pos - curPos3) < 15){break;}
      Servo3.write(pos);
      curPos3 = pos;
      Serial.println("Ring finger moved");
      break;
    case 4:
      if(abs(pos - curPos4) < 15){break;}
      Servo4.write(pos);
      curPos4 = pos;
      Serial.println("Pinky finger moved");
      break;
    case 5:
      if(abs(pos - curPos5) < 15){break;}
      Servo5.write(pos);
      curPos5 = pos;
      Serial.println("Thumb moved");
      break;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  while(counter < 200){
    counter++;
    processData();
    //delay(2000);
    if(counter == 30){
      Serial.println("Done with trial");
    }
  }
 
  
}

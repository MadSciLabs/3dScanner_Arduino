/*
  LAB 3D SCANNER


  by Adam Lassy & Steve Cordova

 */

#include <Bounce2.h>
#include <AccelStepper.h>


// constants won't change. They're used here to
// set pin numbers:
const int buttonPin = 5;     // the number of the pushbutton pin
const int platformPin =  4;      // the number of the LED pin
const int topLimitPin = 7;
const int bottomLimitPin = 6;
const int ledPin = 3;
const int motorDirPin = 9;
const int motorStepPin = 8;


const float FADE_SPEED = .01;

const int STATE_WAIT = 0;
const int STATE_RUN = 1;
const int STATE_DONE = 2;


//set up the accelStepper intance
AccelStepper stepper(AccelStepper::FULL2WIRE, motorStepPin, motorDirPin); 

// button fading vars
float speed = .5;
float fadeValue = 0;

int state = 0;
int platformState = LOW;

Bounce startBtn = Bounce(); 
Bounce topLimitBtn = Bounce();
Bounce bottomLimitBtn = Bounce();

int stepperSpeed = 20000;  

void advanceState()
{
  state = state+1;
  if (state > STATE_DONE) {
    state = 0;
  }
  
  switch(state) {

    case STATE_WAIT:

      platformState = LOW;
      break;

    case STATE_RUN:
    
      writeLEDButton(200);
      
      //stepper.moveTo(500000);
      //stepper.setSpeed(2000);
      
      platformState = HIGH;
      break;

    case STATE_DONE:

      writeLEDButton(0);
      platformState = LOW;
      break;
  }

  //WRITE TO PLATFORM
  digitalWrite(platformPin, platformState);
  digitalWrite(motorDirPin, HIGH);
 
}

void setup() {

  pinMode(ledPin, OUTPUT);
  pinMode(platformPin, OUTPUT);
  pinMode(buttonPin, INPUT);

  pinMode(topLimitPin, INPUT);
  pinMode(bottomLimitPin, INPUT);

  pinMode(motorDirPin, OUTPUT);
  pinMode(motorStepPin, OUTPUT);
  
  startBtn.attach(buttonPin);
  startBtn.interval(50);

  topLimitBtn.attach(topLimitPin);
  topLimitBtn.interval(50);

  bottomLimitBtn.attach(bottomLimitPin);
  bottomLimitBtn.interval(50);
  
  digitalWrite(platformPin, platformState);

  //stepper.setMaxSpeed(1000);
  //stepper.setAcceleration(500);

  stepper.setMaxSpeed(stepperSpeed);
  stepper.setSpeed(stepperSpeed);    
 
  //Serial.begin(9600); 
}

void fadeButton()
{
  fadeValue = fadeValue + speed;

  if (fadeValue >= 150) {
    speed = -FADE_SPEED;
  } else if (fadeValue <= -10) {
    speed = FADE_SPEED;
  }

  writeLEDButton(fadeValue);
  
}

void writeLEDButton(int _val)
{
  if (_val < 0) { _val = 0;}
  analogWrite(ledPin, _val);
}


void loop() {

  startBtn.update();
  topLimitBtn.update();
  bottomLimitBtn.update();

  switch (state) {

    case STATE_WAIT:
      fadeButton();

      //startBtn.read();
      if (startBtn.fell())
      {
        Serial.println("PUSHBUTTON");
        advanceState();
      }
      break;

    case STATE_RUN:


      if (topLimitBtn.fell())
      {
        Serial.println("TOP"); 
        stepper.setSpeed(stepperSpeed);
        //stepper.moveTo(-50000);
        //stepper.setSpeed(200);
      }
      else if (bottomLimitBtn.fell())
      {
        stepper.setSpeed(-stepperSpeed);
        Serial.println("BOTTOM"); 
        //stepper.moveTo(50000);
        //stepper.setSpeed(200);
      }

      //stepper.run();
       stepper.runSpeed();
    
      if (startBtn.fell())
      {
        Serial.println("PUSHBUTTON");
        advanceState();
      }

      break;

    case STATE_DONE:

      //startBtn.read();
      if (startBtn.fell())
      {
        Serial.println("PUSHBUTTON");
        advanceState();
      }
      break;

  }

  //Serial.println(fadeValue);
  

  
}

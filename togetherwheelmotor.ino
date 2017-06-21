#include "DualVNH5019MotorShield.h"
DualVNH5019MotorShield md;

#include <Encoder.h>
Encoder myEnc(2, 3);
// set pin numbers:
const int buttonPin = 5;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

unsigned long storedMillis;
int encCount;

float RPS;
float desiredRPS = 0;
int totalRotations;
int storedRotations;

int motorSpeed = 0;
float motorSpeedA = 30;

int pwmPin = A0;
int pwmVal = 0;

float encRes = 28; // This is the 'pulses per revolution' which can vary between encoders.
float gearDiv =4 ; // This is the Gear ration as a divisor. i.e. A gear reduction ration of 4:1 is a divisor of 4.

float PulleyDrive = 27; // Number of teeth, or circle diameter of pulley attach to Drive/Motor shaft;
float PulleyAxle = 60; // Number of teeth, or circle diameter of pulley attach to Axle;

float reduction = 0;

int TotalEncCount = 0;




void setup()
{

 // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

  Serial.begin(4800);
  md.init(); // required for Pololu 33926 Libary

reduction = (encRes*gearDiv)/(PulleyAxle/PulleyDrive);

}


void loop(){

// read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    // turn LED on:
    digitalWrite(ledPin, HIGH);
    desiredRPS = 9.166;
  } else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
    desiredRPS = 3;
  }

  {

    pwmVal = analogRead(pwmPin);
    motorSpeed = map(pwmVal, 0, 1064, 0, 255);
    md.setM2Speed(-motorSpeedA);
  }


  {

  TotalEncCount = myEnc.read();
    unsigned long NewMillis = millis();



    if (NewMillis - storedMillis > (125)) // every eighth of 1 second
    {
      encCount = (TotalEncCount - storedRotations); // the count for that second is the new rotation amount, minus the old rotation amount.
      // uncomment to report total Rotations per second
      storedRotations = TotalEncCount;
      storedMillis = NewMillis;
      if (encCount >= 0)
      {
        if (RPS > (desiredRPS) && motorSpeedA > 0) motorSpeedA--;
        if (RPS <= (desiredRPS) && motorSpeedA < 160) motorSpeedA++;
      }
      if (encCount < 0)
      {
        if (RPS > (-desiredRPS) && motorSpeedA > 0) motorSpeedA++;
        if (RPS < (-desiredRPS) && motorSpeedA < 160) motorSpeedA--;
      }
Serial.print("Detected RPS: ");
 Serial.println(RPS);
 Serial.print("Output Speed: ");
  Serial.println(motorSpeedA);
  Serial.print("Desired RPS: ");
  Serial.println(desiredRPS);
    }
  }

RPS = (encCount/reduction*8);

}

  

 










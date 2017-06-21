
// Together Wheel
// By Craig Neilson

// Created for MOTAT, Auckland New Zealand, for Matariki 'Night Lights' event.
// Special credits to Ian Neilson, Sammy Williams, Nick Schwarzenberg, Cameron May, John Edmark

// An Arduino-controled rotating zoetrope
// I hereby release this code to the PUBLIC DOMAIN, YEEEHAW

//INSTRUCTIONS:

//Sense press:
//Play new sound
//Send HIGH signal to motor
//Count to 5
//Send relay HIGH signal (turn off) ambient lights
//Flicker strobes
//
//
//No press:
//
//Turn off strobes
//Play original sound
//Send LOW signal to motor
//Send LOW to Relay (ambient lights on)
//
//
//


//------------------------------------------------------------------
// General configuration
//------------------------------------------------------------------

// Set the number of times for the lights to flash per second
unsigned int flashCount = 24;
// And the duraction (milliseconds) that each flash on should be
// 1 second = 1,000 milliseconds
const long onTime = 2; 

// Automatic calculation of the offTime based on the above
unsigned long totalOnTime = onTime * flashCount;
unsigned long totalOffTime = 1000 - totalOnTime;
unsigned long offTime = totalOffTime / flashCount;
//unsigned long currentMillis = millis();
unsigned long currentMillis = 0;
unsigned long buttonMillis = 0;
unsigned long touchDuration = 0;
unsigned long buttonTime; // will store the time the button was pressed


const short triggerThreshold = 150;// Lower value is more sensitive

const int buttonPin = 2;     // the number of the pushbutton pin
int buttonState = 0;         // variable for reading the pushbutton status
const int song1Pin =  3;      // Song 1 on the MP3 module
const int song2Pin =  4;      // Song 2 on the MP3 module
const int relay1Pin = 5;      // Relay channel 1
const int relay2Pin = 6;      // Relay channel 2
const int motorPin = 7;       // Tells the motor to go in to fast mode when high
unsigned int buttonStep = 0;


//------------------------------------------------------------------
// Schedule when button pressed
//------------------------------------------------------------------

unsigned long motorTime = 1000;
unsigned long relayTime = 4000;
unsigned long strobeTime = 6000;

//------------------------------------------------------------------
// Define our ledControl class
//------------------------------------------------------------------

class ledControl {

    // Class that controls the state of the LEDs
    // Doesn't handle timing any more as this needs to be global

    // Private vars to be only used here
    unsigned int ledPin;

    // Public vars to be shared with the rest of the code
    public:
    
    // Constructor - creates an ledControl
    ledControl(int _pin ) {
      ledPin = _pin;
      pinMode(ledPin, OUTPUT);
    }

    // Function to switch state as input
    void switchLedState(unsigned int _state) {
      digitalWrite(ledPin,_state);
    }
    
}; // End definition of ledControl class

//------------------------------------------------------------------
// Configure each LED channel here
//------------------------------------------------------------------


// input vars are (pin, note)
ledControl led1(10);
ledControl led2(11);
ledControl led3(12);
ledControl led4(13);


// Init the vars for global LED control
unsigned int globalLedState = LOW; // Start with our state as lights off
unsigned long ledLastSwitch = 0;
unsigned long curTime = 0;



void setup() {
  // put your setup code here, to run once:

    // Make sure the lights are off when we start:

    led1.switchLedState(LOW);
    led2.switchLedState(LOW);
    led3.switchLedState(LOW);
    led4.switchLedState(LOW);

    // initialize the pushbutton pin as an input:
 
  Serial.begin(4800);
  pinMode(buttonPin, INPUT);

  // Analog touch sensor setup
  pinMode(A0, INPUT);
  pinMode(A5, OUTPUT);
  digitalWrite(A5, LOW);
  delay(1);
    
} // End setup


void loop() {

  currentMillis = millis() ;

// Read analog & generate buttonpress for digital button
short value = analogRead(A0);
  digitalWrite(A5, (value > triggerThreshold) ? HIGH : LOW);
  delayMicroseconds(10000);// 10ms;

buttonState = digitalRead(buttonPin);

 
  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH) {


//------------------------------------------------------------------
// Button is PRESSED, let's party.
//------------------------------------------------------------------


    if (buttonMillis == 0)
{
//buttonMillis = currentMillis; // check and label the time that we pressed the button
}
    // calculate elapsed time since touch
    touchDuration = currentMillis - buttonMillis;
    
    Serial.write("Button ON\n");
    Serial.write(curTime);


    unsigned long buttonMillis = currentMillis; // check and label the time
    // calculate elapsed time since touch
    unsigned long touchDuration = millis() - buttonMillis;
    
    Serial.write("Button ON\n");
    Serial.write(curTime);


     // Play the "active" sound
     pinMode(song1Pin, INPUT);   // disconnect Song1 from ground
     pinMode(song2Pin, OUTPUT); // Play Song2

     // Make the motor speed up
      digitalWrite(motorPin, HIGH);

 if ((buttonStep == 0) & (buttonState == HIGH))
{
delay(8000);
}
buttonStep = 1;


//delay(4000);
//if (touchDuration > 4000)
delay(2);
//    {
     // Switch the relay lights OFF
     digitalWrite(relay1Pin, LOW);
     digitalWrite(relay2Pin, LOW);
//    }

 

//delay(4000);
 //------------------------------------------------------------------
  // Begin LED control
  //------------------------------------------------------------------

    curTime = millis();      
    if (( globalLedState == LOW ) && ( curTime - ledLastSwitch >= offTime )) {
      globalLedState = HIGH;
      ledLastSwitch = curTime;
      {
        led1.switchLedState(HIGH);
        led2.switchLedState(HIGH);
        led3.switchLedState(HIGH);
        led4.switchLedState(HIGH);
      }
    }

    if (( globalLedState == HIGH) && ( curTime - ledLastSwitch >= onTime )) {
      globalLedState = LOW;
      ledLastSwitch = curTime;
      {
        led1.switchLedState(LOW);
        led2.switchLedState(LOW);
        led3.switchLedState(LOW);
        led4.switchLedState(LOW);
      }
    }

        
}

// End of what to do when the button is pressed.

//------------------------------------------------------------------
// What to do if the button is not pressed
//------------------------------------------------------------------

else {
    // Turn things off
    Serial.write("Button OFF\n");
    Serial.write(curTime);
    led1.switchLedState(LOW);
        led2.switchLedState(LOW);
        led3.switchLedState(LOW);
        led4.switchLedState(LOW);

     // Play the "holding" sound
     pinMode(song1Pin, OUTPUT); // Play Song1
     pinMode(song2Pin, INPUT);   // disconnect Song2 from ground

     // Make the motor slow down
     digitalWrite(motorPin, LOW);

     // Reset the timing
     buttonMillis = 0 ;

     // Switch the relay lights ON
     digitalWrite(relay1Pin, HIGH);
     digitalWrite(relay2Pin, HIGH);

     buttonStep = 0;
  }

}

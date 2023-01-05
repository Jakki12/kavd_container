// ---------------------------------------------------------------------------
// Example NewPing library sketch that pings 3 sensors 20 times a second.
//https://bitbucket.org/teckel12/arduino-new-ping/wiki/Home#!ping-3-sensors-sketch
// ---------------------------------------------------------------------------

#include <NewPing.h>
#include "helpfunctions.h"

//DELAY STUFF
unsigned long previousMillis = 0; // speichert den Zeitpunkt an dem zuletzt geschalten wurde
const long myDelay = 50; // Länge der Pause in ms


//GLOBAL STUFF
#define BRETT_INDICATOR 1
int index = 0;


//SENSOR STUFF
#define SONAR_NUM 2      // Number of sensors.
#define MAX_DISTANCE 180 // Maximum distance (in cm) to ping.
#define MIN_DISTANCE 3  // Minimum distance (in cm) to constrain for

/*
NewPing sonar[SONAR_NUM] = {   // Sensor object array.
  NewPing(22, 24, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping. 
  NewPing(22, 25, MAX_DISTANCE), 
  NewPing(22, 26, MAX_DISTANCE),
  NewPing(22, 27, MAX_DISTANCE),
  NewPing(22, 28, MAX_DISTANCE),
  NewPing(22, 29, MAX_DISTANCE),
  NewPing(22, 30, MAX_DISTANCE),
  NewPing(22, 31, MAX_DISTANCE),
  NewPing(22, 32, MAX_DISTANCE)
};
*/


//NewPing sonar[SONAR_NUM] = {   // Sensor object array.
//  NewPing(22, 24, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping. 
//  NewPing(22, 25, MAX_DISTANCE), 
//  NewPing(22, 26, MAX_DISTANCE),
//  NewPing(22, 27, MAX_DISTANCE)
//};


NewPing sonar[SONAR_NUM] = {   
  NewPing(22, 25, MAX_DISTANCE),
  NewPing(40, 36, MAX_DISTANCE)
};



//SEND STUFF
const int sensValuesArraySize = SONAR_NUM+2; //+2 for indicator values that indicate start of the bytestream and brett-id
byte sensValues[sensValuesArraySize];

//RECEIVE STUFF
const unsigned int MAX_MESSAGE_LENGTH = 10; //max allowed length of incoming dim-val data
char terminatingChar = '\@'; //define terminating character for receive-loop
//const int pwmPins[SONAR_NUM] = {2, 3, 4, 5, 6, 7, 8, 9, 10};
//const int pwmPins[SONAR_NUM] = {2, 3, 4, 5};
const int pwmPins[SONAR_NUM] = {3, 4};

const byte numChars = 15;
char receivedChars[numChars];

boolean newData = false;




void setup() {


  //DEBUG STUFF
  pinMode(48, OUTPUT);
  
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  sensValues[0] = 0; //first byte of sensValue-Array is 0 to indicate start of byteStream for the receiver
  sensValues[1] = BRETT_INDICATOR; //indicating brett-number (1-8, must not be 0)

  //init pwm pins
  for (int p = 0; p < SONAR_NUM; p++)
  {
    pinMode(pwmPins[p], OUTPUT);
    analogWrite(pwmPins[p], 0); //turn off all pwmPins on start up
  }
}


void loop() {

  
  unsigned long currentMillis = millis(); // Aktuelle Zeit wird in currentMillis gespeichert

  if (currentMillis - previousMillis >= myDelay) // Alle 50 ms pingen
  { 

    for(int s=0; s<SONAR_NUM; s++)
    {
      sensValues[s+SONAR_NUM]=1;
      
      float distance = sonar[s].ping_cm();
      
      if(distance<=0) //distance ist 0.00, wenn MAX_DISTANCE überschritten wird
      {
        distance = MAX_DISTANCE;
      }
      
      float constrainval = constrain(distance, MIN_DISTANCE, MAX_DISTANCE);
      
      byte byteval = map(constrainval, MIN_DISTANCE, MAX_DISTANCE, 255, 1);   //mapping: (value, fromLow, fromHigh, toLow, toHigh)
      
      sensValues[s+SONAR_NUM] = byteval; // save current sens-value into array; offset because of both the indicator values        
    }

    Serial.write(sensValues, sizeof(sensValues)); //send away current sensor values to PD

    previousMillis = currentMillis; // Zeitpunkt der letzten Schaltung wird festgehalten


  }

    //RECEIVE
    recvWithStartEndMarkers();
    showNewData();


    int lampIdRecv = getNumBetweenChars(receivedChars, sizeof(receivedChars), 'L', 'V');
    int dimValRecv = getNumBetweenChars(receivedChars, sizeof(receivedChars), 'V', '\0');
        analogWrite(pwmPins[lampIdRecv-1], dimValRecv);


}//loop



void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = 'B';
    char endMarker = '@';
    char rc;
 
    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

void showNewData() {
    if (newData == true) {
//        Serial.print("This just in ... ");
//        Serial.println(receivedChars);
//        Serial.print("Lamp id: ");
//        Serial.println(getNumBetweenChars(receivedChars, sizeof(receivedChars), 'L', 'V'));
//        Serial.print("dim val: ");
//        Serial.println(getNumBetweenChars(receivedChars, sizeof(receivedChars), 'V', '\0'));
        newData = false;
    }
}

// ---------------------------------------------------------------------------
// Example NewPing library sketch that pings 3 sensors 20 times a second.
//https://bitbucket.org/teckel12/arduino-new-ping/wiki/Home#!ping-3-sensors-sketch
// ---------------------------------------------------------------------------

#include <NewPing.h>
#include "helpfunctions.h"

//GLOBAL STUFF
#define BRETT_INDICATOR 1


//SENSOR STUFF
#define SONAR_NUM 9      // Number of sensors.
#define MAX_DISTANCE 50 // Maximum distance (in cm) to ping.
#define MIN_DISTANCE 3  // Minimum distance (in cm) to constrain for

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


//SEND STUFF
const int sensValuesArraySize = SONAR_NUM+2; //+2 for indicator values that indicate start of the bytestream and brett-id
byte sensValues[sensValuesArraySize];

//RECEIVE STUFF
const unsigned int MAX_MESSAGE_LENGTH = 20; //max allowed length of incoming dim-val data
char terminatingChar = '\@'; //define terminating character for receive-loop
const int pwmPins[SONAR_NUM] = {2, 3, 4, 5, 6, 7, 8, 9, 10};
//const int pwmPins[SONAR_NUM] = {2, 3, 4};


void setup() {
  Serial.begin(9600); // Open serial monitor at 115200 baud to see ping results.
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
    analogWrite(pwmPins[p], 0); //turn of all pwmPins on start up
  }
  
}

void loop() { 
  for (uint8_t i = 0; i < SONAR_NUM; i++) { // Loop through each sensor and display results.
    delay(50); // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
    float distance = sonar[i].ping_cm();
    if(distance<=0) //distance ist 0.00, wenn MAX_DISTANCE überschritten wird
    {
      distance = MAX_DISTANCE;
    }
    float constrainval = constrain(distance, MIN_DISTANCE, MAX_DISTANCE);
    byte byteval = map(constrainval, MIN_DISTANCE, MAX_DISTANCE, 255, 1);   //mapping: (value, fromLow, fromHigh, toLow, toHigh)
    sensValues[i+2] = byteval; // save current sens-value into array; offset because of both the indicator values
    
//    Serial.print(i);
//    Serial.print("=");
//    Serial.print(byteval);
//    Serial.print(" ");


  }
  //Serial.println();
   Serial.write(sensValues, sizeof(sensValues)); //send away current sensor values to PD






    //RECEIVE
   while (Serial.available() > 0) //while loop gut????
  {
    static char message[MAX_MESSAGE_LENGTH]; //array to store data from serial buffer
    static unsigned int message_pos = 0; //position variable for moving through array

    //read the next available byte from the serial buffer
    char inByte = Serial.read();

    //check if byte is terminating character and if incoming data was too long for our array
    if (inByte != terminatingChar && (message_pos < MAX_MESSAGE_LENGTH - 1))
    {
      //still reading message, put data into array
      message[message_pos] = inByte;
      message_pos++;
    }
    else
    {
      //we have a terminating character, full message received
      //Add null character to string, necessary for atoi() to work
      message[message_pos] = '\0';
      //Print the message
      message_pos = 0;

      
      int brettIdReceived = getNumBetweenChars(message, sizeof(message), 'B', 'L');
      int LampIdReceived = getNumBetweenChars(message, sizeof(message), 'L', 'V');
      int DimValReceived = getNumBetweenChars(message, sizeof(message), 'V', '\0');
     
      if(BRETT_INDICATOR == brettIdReceived)
      {
        analogWrite(pwmPins[LampIdReceived-1], DimValReceived);
      }
       
    }//else
  }//while
}//loop

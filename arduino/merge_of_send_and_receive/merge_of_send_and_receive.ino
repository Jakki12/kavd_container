#include <HCSR04.h> 
#include "helpfunctions.h"

// https://github.com/gamegine/HCSR04-ultrasonic-sensor-lib


//TESTING STUFF
int ledPin = 8;
const unsigned int MAX_MESSAGE_LENGTH = 20;
static char message[MAX_MESSAGE_LENGTH]; //array to store data from serial buffer
static unsigned int message_pos = 0; //position variable for moving through array

char brettIdChar = '1'; //must start at 1


//GLOBAL STUFF
int brettId = 1;
const int sensorAmount = 9;
const int pwmPinAmount = 9;



//SEND STUFF//
int triggerPin = 22;
const int arraySize = sensorAmount+2; //+2 for indicator values that indicate start of the bytestream and brett-id
byte sensValues[arraySize];
int max_distance = 120; //cm
float test = 120;
int min_distance = 30; //cm
int echoPins[sensorAmount] = {24, 25, 26, 27, 28, 29, 30, 31, 32};
HCSR04 hc(triggerPin, echoPins, sensorAmount); //initialisation class HCSR04 (trig pin , echo pins, number of sensors)



//RECEIVE STUFF
//const unsigned int MAX_MESSAGE_LENGTH = 20; //max allowed length of incoming dim-val data
char terminatingChar = '\n'; //define terminating character for receive-loop
const int pwmPins[pwmPinAmount] = {2, 3, 4, 5, 6, 7, 8, 9, 10};


void setup()
{ 
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  //SEND STUFF
  sensValues[0] = 0; //first byte of sensValue-Array is 0 to indicate start of byteStream for the receiver
  sensValues[1] = brettId; //indicating brett-number (1-8, must not be 0)

  //RECEIVE STUFF
  for (int p = 0; p < pwmPinAmount; p++)
  {
    pinMode(pwmPins[p], OUTPUT);
    analogWrite(pwmPins[p], 0); //turn of all pwmPins on start up
  }
}

void loop()
{   
  
  //SENSOR PROCESSING AND SENDING TO PD
  for (int i = 0; i < sensorAmount ; i++ ) //for each sensor
  {
    float distance = hc.dist(i);
    if(distance == 0)
    { 
      distance = max_distance;    
    }
    
    float constrainval = constrain(distance, min_distance, max_distance); //  
    byte byteval = map(constrainval, min_distance, max_distance, 255, 1);   //mapping: (value, fromLow, fromHigh, toLow, toHigh)
    sensValues[i+2] = byteval; // offset because of both the indicator values
  }
  Serial.write(sensValues, sizeof(sensValues));
  delay(60);// we suggest to use over 60ms measurement cycle, in order to prevent trigger signal to the echo signal.




  //RECEIVING FROM PD AND ROUTING TO LAMPS
  while (Serial.available() > 0)
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
      Serial.println(message); 
      message_pos = 0;

      
      int brettIdReceived = getNumBetweenChars(message, sizeof(message), 'B', 'L');
      Serial.println("brettIdReceived: ");
      Serial.println(brettIdReceived);

      int LampIdReceived = getNumBetweenChars(message, sizeof(message), 'L', 'V');
      Serial.println("LampIdReceived: ");
      Serial.println(LampIdReceived);


      int DimValReceived = getNumBetweenChars(message, sizeof(message), 'V', '\0');
      Serial.println("DimValReceived: ");
      Serial.println(DimValReceived);


      if(brettId == brettIdReceived)
      {
        analogWrite(pwmPins[LampIdReceived-1], DimValReceived);
      }
       
    }//else
  }//while

  

//  
//  int newBytesAmount = Serial.available();
//
//  if(newBytesAmount>0)
//  {
//    int newByte = Serial.read();
//      Serial.println((char)newByte);
//
//  }

   

   
    //int brettIdReceived = getNumBetweenChars(buf, sizeof(buf), 'B', 'L');

    //int lampIdReceived = getNumBetweenChars(buf, sizeof(buf), 'L', 'V');


  
  
  
}//loop

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
#define SONAR_NUM 6      // Number of sensors.
#define MAX_DISTANCE 500 // Maximum distance (in cm) to ping.
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
//  NewPing(22, 42, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping. 
//  NewPing(22, 43, MAX_DISTANCE), // 42,43 = fake
//  NewPing(22, 24, MAX_DISTANCE), //24 = loser Sensor
//  NewPing(22, 25, MAX_DISTANCE) //25 = Wandsensor
//};


//NewPing sonar[SONAR_NUM] = {   
//  NewPing(22, 24, MAX_DISTANCE),
//  NewPing(22, 25, MAX_DISTANCE)
//};

NewPing sensor1 = NewPing(22,24,MAX_DISTANCE); 
NewPing sensor2 = NewPing(22,25,MAX_DISTANCE);
NewPing sensor3 = NewPing(22,26,MAX_DISTANCE);
NewPing sensor4 = NewPing(22,27,MAX_DISTANCE);
NewPing sensor5 = NewPing(22,28,MAX_DISTANCE);
NewPing sensor6 = NewPing(22,29,MAX_DISTANCE);







//SEND STUFF
const int sensValuesArraySize = SONAR_NUM+2; //+2 for indicator values that indicate start of the bytestream and brett-id
byte sensValues[sensValuesArraySize];

//RECEIVE STUFF
const unsigned int MAX_MESSAGE_LENGTH = 10; //max allowed length of incoming dim-val data
char terminatingChar = '\@'; //define terminating character for receive-loop
//const int pwmPins[SONAR_NUM] = {2, 3, 4, 5, 6, 7, 8, 9, 10};
//const int pwmPins[SONAR_NUM] = {2, 3, 4, 5}; //2,3 = fake, 4=loser Sensor, 5=Wandsensor

const int pwmPins[SONAR_NUM] = {4, 5, 6, 7, 8, 9}; 


const byte numChars = 15;
char receivedChars[numChars];
boolean newData = false;




void setup() {


  //DEBUG STUFF
  pinMode(44, OUTPUT);
  
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
    //pinMode(pwmPins[p], OUTPUT);
    analogWrite(pwmPins[p], 0); //turn off all pwmPins on start up
  }
}


void loop() {

  digitalWrite(44,LOW);
  unsigned long currentMillis = millis(); // Aktuelle Zeit wird in currentMillis gespeichert

  if (currentMillis - previousMillis >= myDelay) // Alle 50 ms pingen
  { 

//    for(int s=0; s<SONAR_NUM; s++)
//    {
//      //sensValues[s+SONAR_NUM]=1;
//      
//      float distance = sonar[s].ping_cm();
//      
//      if(distance<=0) //distance ist 0.00, wenn MAX_DISTANCE überschritten wird
//      {
//        distance = MAX_DISTANCE;
//      }
//      
//      float constrainval = constrain(distance, MIN_DISTANCE, MAX_DISTANCE);
//      
//      byte byteval = map(constrainval, MIN_DISTANCE, MAX_DISTANCE, 255, 1);   //mapping: (value, fromLow, fromHigh, toLow, toHigh)
//      
//      sensValues[s+SONAR_NUM] = byteval; // save current sens-value into array; offset because of both the indicator values        
//    }

//    sensValues[2] = 1; //gib den fake sensoren eine 1. sonst wäre es 0 und dann würde die empfangsroutine nicht funktionieren.
//    sensValues[3] = 1;



    //Serial.write(sensValues, sizeof(sensValues)); //send away current sensor values to PD


//
    float distance_sensor1 = sensor1.ping_cm();

    if(distance_sensor1 <= 0)
    {
      distance_sensor1 = MAX_DISTANCE;
    }

    float distance_1_constr  = constrain(distance_sensor1, MIN_DISTANCE, MAX_DISTANCE);

    byte sensor1_byte = map(distance_1_constr, MIN_DISTANCE, MAX_DISTANCE, 255, 1);   //mapping: (value, fromLow, fromHigh, toLow, toHigh)

//    
    float distance_sensor2 = sensor2.ping_cm();
    
    if(distance_sensor2 <= 0)
    {
      distance_sensor2 = MAX_DISTANCE;
    }
   

    float distance_2_constr = constrain(distance_sensor2, MIN_DISTANCE, MAX_DISTANCE);

    byte sensor2_byte = map(distance_2_constr, MIN_DISTANCE, MAX_DISTANCE, 255, 1);   //mapping: (value, fromLow, fromHigh, toLow, toHigh)

//

    float distance_sensor3 = sensor3.ping_cm();
    
    if(distance_sensor3 <= 0)
    {
      distance_sensor3 = MAX_DISTANCE;
    }
    
    float distance_3_constr = constrain(distance_sensor3, MIN_DISTANCE, MAX_DISTANCE);

    byte sensor3_byte = map(distance_3_constr, MIN_DISTANCE, MAX_DISTANCE, 255, 1);   //mapping: (value, fromLow, fromHigh, toLow, toHigh)
//
    
    float distance_sensor4 = sensor4.ping_cm();
    
    if(distance_sensor4 <= 0)
    {
      distance_sensor4 = MAX_DISTANCE;
    }
    float distance_4_constr = constrain(distance_sensor4, MIN_DISTANCE, MAX_DISTANCE);

    byte sensor4_byte = map(distance_4_constr, MIN_DISTANCE, MAX_DISTANCE, 255, 1);   //mapping: (value, fromLow, fromHigh, toLow, toHigh)

//

    float distance_sensor5 = sensor5.ping_cm();
    
    if(distance_sensor5 <= 0)
    {
      distance_sensor5 = MAX_DISTANCE;
    }

    float distance_5_constr = constrain(distance_sensor5, MIN_DISTANCE, MAX_DISTANCE);

    byte sensor5_byte = map(distance_5_constr, MIN_DISTANCE, MAX_DISTANCE, 255, 1);   //mapping: (value, fromLow, fromHigh, toLow, toHigh)

//

    float distance_sensor6 = sensor6.ping_cm();
    
    if(distance_sensor6 <= 0)
    {
      distance_sensor6 = MAX_DISTANCE;
    }
    float distance_6_constr = constrain(distance_sensor6, MIN_DISTANCE, MAX_DISTANCE);

    byte sensor6_byte = map(distance_6_constr, MIN_DISTANCE, MAX_DISTANCE, 255, 1);   //mapping: (value, fromLow, fromHigh, toLow, toHigh)
//

    
    sensValues[2] = sensor1_byte;
    sensValues[3] = sensor2_byte;
    sensValues[4] = sensor3_byte;
    sensValues[5] = sensor4_byte;
    sensValues[6] = sensor5_byte;
    sensValues[7] = sensor6_byte;
    

   
    Serial.write(sensValues, sizeof(sensValues)); //send away current sensor values to PD


      

//    Serial.print("wandsensor: ");
//    Serial.println(distance_wandsensor);
//    
//    Serial.print("loser sensor: ");
//    Serial.println(distance_loserSensor);

//    Serial.print("wandsensor: ");
//    Serial.println(sensValues[2]);
//    
//    Serial.print("loser sensor: ");
//    Serial.println(sensValues[3]);


    
    
    previousMillis = currentMillis; // Zeitpunkt der letzten Schaltung wird festgehalten


  }

    //RECEIVE
    recvWithStartEndMarkers();
    showNewData();

//
//    int lampIdRecv = receivedChars[0];
//    if(lampIdRecv == 1)
//    {
//      digitalWrite(44,HIGH);
//    }
//    else{
//      digitalWrite(44,LOW);
//    }
//    int dimValRecv = getNumBetweenChars(receivedChars, sizeof(receivedChars), 'V', '\0');
//    //analogWrite(pwmPins[lampIdRecv-1], dimValRecv);

      
      //int lampIdRecv = receivedChars[0]-'0';
      //int dimValRecv = getNumBetweenChars(receivedChars, sizeof(receivedChars), 'V', '\0');


//      if (lampIdRecv == 1)
//      {
//        if(dimValRecv > 200)
//        {
//          digitalWrite(44, HIGH);
//          analogWrite(pwmPins[0], dimValRecv);
//        }
//        else
//        {
//        analogWrite(pwmPins[0], 1);
//
//        }
//      
//          
//      }
//      else if(lampIdRecv == 2)
//      {
//        digitalWrite(44, LOW);
//      }
      



      //analogWrite(pwmPins[lampIdRecv-1], dimValRecv);

//Serial.println(receivedChars);
//int test = 
//Serial.println(test);
//if(test == 1)
//{Serial.println("success");}

    
}//loop




void recvWithStartEndMarkers() {

  
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = 'L';
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

        int id = receivedChars[0]-'0';
        int dimVal = getNumBetweenChars(receivedChars, sizeof(receivedChars), 'V', '\0');

        if (id==1)
        {
          analogWrite(pwmPins[0], dimVal);
        }else if (id==2)
        {
          analogWrite(pwmPins[1], dimVal);
        }else if (id==3)
        {
          analogWrite(pwmPins[2], dimVal);
        }else if (id==4)
        {
          analogWrite(pwmPins[3], dimVal);
        }else if (id==5)
        {
          analogWrite(pwmPins[4], dimVal);
        }else if (id==6)
        {
          analogWrite(pwmPins[5], dimVal);
        }
        

        newData = false;
    }
}

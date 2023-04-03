// ---------------------------------------------------------------------------
// Example NewPing library sketch that pings 3 sensors 20 times a second.
// https://bitbucket.org/teckel12/arduino-new-ping/wiki/Home#!ping-3-sensors-sketch
// ---------------------------------------------------------------------------

#include <NewPing.h>

//GLOBAL STUFF
#define BRETT_INDICATOR 1


//SENSOR STUFF
#define SONAR_NUM 3      // Number of sensors.
#define MAX_DISTANCE 200 // Maximum distance (in cm) to ping.
#define MIN_DISTANCE 3  // Minimum distance (in cm) to constrain for

NewPing sonar[SONAR_NUM] = {   // Sensor object array.
  NewPing(22, 24, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping. 
  NewPing(22, 25, MAX_DISTANCE), 
  NewPing(22, 26, MAX_DISTANCE)
};


//SEND STUFF
const int sensValuesArraySize = SONAR_NUM+2; //+2 for indicator values that indicate start of the bytestream and brett-id
byte sensValues[sensValuesArraySize];



void setup() {
  Serial.begin(9600); // Open serial monitor at 115200 baud to see ping results.
  sensValues[0] = 0; //first byte of sensValue-Array is 0 to indicate start of byteStream for the receiver
  sensValues[1] = BRETT_INDICATOR; //indicating brett-number (1-8, must not be 0)
  
}

void loop() { 
  for (uint8_t i = 0; i < SONAR_NUM; i++) { // Loop through each sensor and display results.
    delay(50); // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
    float distance = sonar[i].ping_cm();
    //Serial.println(distance);
    float constrainval = constrain(distance, MIN_DISTANCE, MAX_DISTANCE);
    byte byteval = map(constrainval, MIN_DISTANCE, MAX_DISTANCE, 255, 1);   //mapping: (value, fromLow, fromHigh, toLow, toHigh)
    sensValues[i+2] = byteval; // save current sens-value into array; offset because of both the indicator values
    
//    Serial.print(i);
//    Serial.print("=");
//    Serial.print(sensValues[i+2]);
//    Serial.print(" ");


  }
  //Serial.println();
   Serial.write(sensValues, sizeof(sensValues));

}

#include <HCSR04.h> 
// https://github.com/gamegine/HCSR04-ultrasonic-sensor-lib


int brett_indicator = 1; //must start at 1
int triggerPin = 22;
const int sensorAmount = 9;

const int arraySize = sensorAmount+2; //+2 for indicator values that indicate start of the bytestream and brett-id

byte sensValues[arraySize];

int max_distance = 120; //cm
float test = 120;
int min_distance = 30; //cm


int echoPins[sensorAmount] = {24, 25, 26, 27, 28, 29, 30, 31, 32};


HCSR04 hc(triggerPin, echoPins, sensorAmount); //initialisation class HCSR04 (trig pin , echo pins, number of sensors)


void setup()
{ 
  Serial.begin(9600);
  sensValues[0] = 0; //first byte of sensValue-Array is 0 to indicate start of byteStream for the receiver
  sensValues[1] = brett_indicator; //indicating brett-number (1-8, must not be 0)
}

void loop()
{   
  
  //SENSOR PROCESSING
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

  
}

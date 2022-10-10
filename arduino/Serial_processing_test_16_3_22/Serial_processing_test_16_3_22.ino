/*
   This sketch builds upon the "Serial_receive_test_16_3_22"

   Now the numbers for the pwm lamps do not come from the keyboard, but from PD

   The form is B1L14V255 or B1L1V1

   B : Brett-id
   L : Lamp-id
   V : dimVal

   In this sketch we have to extract the numbers of that String and send it to the corresponding lamp

   16/3/22
*/

#include "helpfunctions.h"

const unsigned int MAX_MESSAGE_LENGTH = 20; //max allowed length of incoming dim-val data
char terminatingChar = '@'; //define terminating character for receive-loop


const int pwmPinAmount = 9;
const int pwmPins[pwmPinAmount] = {2, 3, 4, 5, 6, 7, 8, 9, 10};

int brettId = 1;

void setup() {

  Serial.begin(9600);

  //init pwm pins
  for (int p = 0; p < pwmPinAmount; p++)
  {
    pinMode(pwmPins[p], OUTPUT);
    analogWrite(pwmPins[p], 0); //turn of all pwmPins on start up
  }

}

void loop() {

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
      
      
    }
  }
    


}

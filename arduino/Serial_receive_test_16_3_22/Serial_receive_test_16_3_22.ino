/*
 * This sketch takes numbers from the serial monitor and writes them to the pwmPins
 * 
 * Test sketch for theLAB-mosfet-controller
 * 
 * 16/3/22
 */


const unsigned int MAX_MESSAGE_LENGTH = 12;


const int pwmPinAmount = 9;
const int pwmPins[pwmPinAmount] = {2,3,4,5,6,7,8,9,10};

void setup() {

 Serial.begin(9600);

 //init pwm pins
 for (int p=0; p<pwmPinAmount; p++)
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

    //read the next available byte in the serial buffer
    char inByte = Serial.read();

    //check if byte is terminating character and if incoming data was too long for our array
    if (inByte != '\n' && (message_pos < MAX_MESSAGE_LENGTH - 1))
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

    //convert string to integer
    int number = atoi(message);

    //Print the number
    Serial.println(number);

    //write number to pwm pin
    //constrain to max pwm value of 255
    if (number > 255)
    {
      number = 255;
    }
    
    for(int i=0; i<pwmPinAmount; i++)
    {
      analogWrite(pwmPins[i], number);  
    }  

    //Reset position variable
    message_pos = 0;
    }
  }
 
 }

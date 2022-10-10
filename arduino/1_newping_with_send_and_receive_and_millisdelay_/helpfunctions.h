
//getting the index of a given char in a char array (!stops at first occurence!)
int getIndexOf(char *arrayToTest, int array_len, char c)
{

  for(int p=0; p<array_len; p++)
  {
    //Serial.println(array[p]);
    if(arrayToTest[p] == c)
    {
      return p;
    } 
  }
  return -1; 
}

//extract numbers between two given chars out of char array
int getNumBetweenChars(char *arrayToCheck, int len, char firstChar, char secondChar)
{
  
  //get indices of the given chars in array
  int startIndexFirstChar = getIndexOf(arrayToCheck, len, firstChar);  
  int startIndexSecondChar = getIndexOf(arrayToCheck, len, secondChar);

  int numberOfDigits = (startIndexSecondChar-1)-(startIndexFirstChar+1)+1; //number of digits between the two chars
  char wantedNumberTmp[numberOfDigits+1]; //+1 for 0 terminator
  
  int wantedNumberLoopIndex = 0;
  
  for(int array_p=startIndexFirstChar+1; array_p<=(startIndexSecondChar-1); array_p++) //for "numberOfDigits" between given characters
  {
    //put the single digits into our temporary array one after another
    wantedNumberTmp[wantedNumberLoopIndex] = arrayToCheck[array_p]; 
    wantedNumberLoopIndex++; 
    if(array_p == (startIndexSecondChar-1)) //put 0 terminator after wanted data for atoi() to work.
    {
      wantedNumberTmp[wantedNumberLoopIndex++] = '\0'; 
    }
  } 
  int wantedNumber  = atoi(wantedNumberTmp);
  return wantedNumber;
} 

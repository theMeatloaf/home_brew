//All Temp and Time Holder Variables:
 static unsigned int elapSecs, lastSec, finalSecs, thisSec;
 static int displayHours, displayMins, displaySecs, controlPin;
 static double temp;

void SetupHoldTemp(int inputPin, double tempy, unsigned int hours, unsigned int mins, unsigned int secs)
{
  temp = tempy;
  controlPin = inputPin;
  
  displayHours = 0;
  displayMins = 0;
  displaySecs = 0;
  elapSecs = 0;
  lastSec  = millis()/1000;//start last sec counter
  
  //convert to final Seconds...works out to about 18.2 hours max if you want...
  finalSecs = (hours*60*60)+(mins*60)+(secs);
}

boolean HoldTempDone()
{  
  if(elapSecs < finalSecs)
  {   
  thisSec = (millis()/1000);
  if(thisSec > lastSec)//if second has passed...display
    {
      lastSec = thisSec;//increment last
      elapSecs++;
    }
    PIDLoop(temp,controlPin);//hold temp at F temp
    return false;
  }else
  {
  Serial.println("");
  Serial.println("");
  Serial.println("TIME PERIOD END!!");
  return true;  
  }
}

///////////display funtions for time since setup Temp
void setupTempController() 
{
  startTempControl();
}

int getDisplayHours()
{
    return elapSecs/60/60;
}

int getDisplayMins()
{
  return (elapSecs-(getDisplayHours()*60*60))/60;
}

int getDisplaySecs()
{
  return elapSecs-(getDisplayHours()*60*60)-(getDisplayMins()*60);
}

//Get toal elapsed
unsigned int getElapsed()
{
 return elapSecs; 
}

//serial debug Display
void displayTimeAndTemp()
{
      displayHours = elapSecs/60/60;
      displayMins = (elapSecs-(displayHours*60*60))/60;
      displaySecs = elapSecs-(displayHours*60*60)-(displayMins*60);
      
      Serial.println();
      Serial.print("Time Elapsed: ");
      Serial.print(displayHours);
      Serial.print(":");
      Serial.print(displayMins);
      Serial.print(":");
      Serial.print(displaySecs);
  
      tempControllerPrint();
}


//All Temp and Time Holder Variables:
 static unsigned int elapSecs, lastSec, finalSecs, thisSec, totalElapSecs;
 static int displayHours, displayMins, displaySecs, controlPin, displayHoursF, displayMinsF, displaySecsF;
 static double temp;
 
 //inport golbal vars
 extern DeviceAddress spargeTherm, mashTherm, wortTherm; 


void SetupHoldTemp(int inputPin, double tempy, unsigned int totalSecs)
{
  temp = tempy;
  controlPin = inputPin;

  displayHoursF = (int)totalSecs/60/60;
  displayMinsF = (int)((totalSecs - (displayHoursF*60*60))/60);
  displaySecsF = (totalSecs - (displayHoursF*60*60) - (displayMinsF*60));

  displayHours = 0;
  displayMins = 0;
  displaySecs = 0;
  elapSecs = 0;
  lastSec  = millis()/1000;//start last sec counter
  
  finalSecs = totalSecs;
}

void SetupHoldTemp(int inputPin, double tempy, unsigned int hours, unsigned int mins, unsigned int secs)
{
  temp = tempy;
  controlPin = inputPin;
  
  displayHoursF = hours;
  displayMinsF = mins;
  displaySecsF = secs;
  
  displayHours = 0;
  displayMins = 0;
  displaySecs = 0;
  elapSecs = 0;
  lastSec  = millis()/1000;//start last sec counter
  
  //convert to final Seconds...works out to about 18.2 hours max if you want...
  finalSecs = (hours*60*60)+(mins*60)+(secs);
}

void forceHoldTemp(int vessel)
{
  thisSec = (millis()/1000);
  if(thisSec > lastSec)//if second has passed...display
    {
      lastSec = thisSec;//increment last
      totalElapSecs++;
    }
    PIDLoop(temp, controlPin, vessel);//hold temp at F temp
}

boolean HoldTempDone(int vessel)
{  
  if(elapSecs < finalSecs)
  {   
  thisSec = (millis()/1000);
  if(thisSec > lastSec)//if second has passed...display
    {
      lastSec = thisSec;//increment last
      elapSecs++;
      totalElapSecs++;
    }
    PIDLoop(temp, controlPin, vessel);//hold temp at F temp
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

int getFinalHours()
{
  return displayHoursF; 
}

int getFinalMins()
{
     return displayMinsF; 
}

int getFinalSecs()
{
     return displaySecsF; 
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

int convertToDisHours(unsigned int inputSecs)
{
  return (int)inputSecs/60/60;
}

int convertToDisMins(unsigned int inputSecs)
{
    return (int)((inputSecs - (convertToDisHours(inputSecs)*60*60))/60);
}

int convertToDisSecs(unsigned int inputSecs)
{
    return (inputSecs - (convertToDisHours(inputSecs)*60*60) - (convertToDisMins(inputSecs)*60));
}

//Get toal elapsed
unsigned int getElapsed()
{
 return elapSecs; 
}

unsigned int getAllElapsed()
{
 return  totalElapSecs;
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




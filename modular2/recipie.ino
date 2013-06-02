#include <EEPROM.h>
#include "EEPROMAnything.h"

//mash variables
#define maxMashes 10
#define secondsPerQuart 9.825 

//static counters and flags;
static unsigned int valveOpenSecs = 0;
static unsigned int thisValveSec = 0;
static unsigned int lastValveSec = 0;
static int currentHopStep = 1;
static int currentMashStep = 0;
static boolean waitFlag = false;
static int lastSeco = 0;

struct recipie{
 //first mash step is strike conditions....so temp and ammount to be poured in...set motor OFF always!
 //last mash step temp is sparge conditions...so temp for sparge...ammount held in diff Variable
 int numberOfMashSteps;
 float mashTemps[maxMashes];
 boolean mashMotorStates[maxMashes];
 float mashAmmounts[maxMashes];
 unsigned int mashTimes[maxMashes];
 unsigned int wortTotalSecs;
 unsigned int hopAdditionIntervals[3];
 float wortTemp;
 char name[17];
 int numOfHopSteps;
};

//this recipie
static recipie curRecipie;

void setCurrentRecipie(struct recipie inputRecipie)
{
 curRecipie = inputRecipie; 
}

struct recipie emptyRecipie()
{
 recipie blankR;
 blankR.numberOfMashSteps = 0;
 for(int i=0; i<maxMashes; i++)
 {
    blankR.mashTemps[i] = 0;
    blankR.mashAmmounts[i] = 0;
    blankR.mashTimes[i] = 0;
    blankR.mashMotorStates[i] = false;
 }
 blankR.wortTemp = 198.0;
 blankR.numOfHopSteps = 0;
 blankR.hopAdditionIntervals[0] = 0;
 blankR.hopAdditionIntervals[1] = 0;
 blankR.hopAdditionIntervals[2] = 0;
  
 for(int i =0; i<17; i++)
 {
  blankR.name[i] = ' ';
 } 
  
 return blankR;
}

void setupTestRecipie()
{
curRecipie.numberOfMashSteps = 3;
curRecipie.mashTemps[0] = -2000;
curRecipie.mashTemps[1] = 130;
curRecipie.mashTemps[2] = 69;
curRecipie.mashTemps[3] = 69;
curRecipie.mashMotorStates[0] = false;
curRecipie.mashMotorStates[1] = false;
curRecipie.mashMotorStates[2] = true;
curRecipie.mashMotorStates[3] = false;
curRecipie.mashTimes[0] = 4294967294;
curRecipie.mashTimes[1] = 10;
curRecipie.mashTimes[2] = 10;
curRecipie.mashTimes[3] = 00;
curRecipie.mashAmmounts[0] = 5;
curRecipie.mashAmmounts[1] = 6;
curRecipie.mashAmmounts[2] = 0;
curRecipie.mashAmmounts[3] = 8;
curRecipie.wortTotalSecs = 3600;
curRecipie.hopAdditionIntervals[0] = 10;
curRecipie.hopAdditionIntervals[1] = 20;
curRecipie.hopAdditionIntervals[2] = 30;
curRecipie.wortTemp = 100;
curRecipie.numOfHopSteps = 3;

curRecipie.name[0] = 't';
curRecipie.name[1] = 'e';
curRecipie.name[2] = 's';
curRecipie.name[3] = 't';
curRecipie.name[4] = 'y';
curRecipie.name[5] = '2';
curRecipie.name[6] = '2';
curRecipie.name[7] = '2';
curRecipie.name[8] = '2';
curRecipie.name[9] = '2';
curRecipie.name[10] = '2';
curRecipie.name[11] = '2';
curRecipie.name[12] = '2';
curRecipie.name[13] = '2';
curRecipie.name[14] = '2';
curRecipie.name[15] = '3';
curRecipie.name[16] = '3';

 
//test!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
Serial.print("THIS IS THE ADDRESS");
//Serial.print((1+(0*4))*142);
Serial.print(EEPROM_writeAnything(((1+(0*4))*142), curRecipie));
Serial.println();
/*for(int i=0; i<4; i++)
  {
    EEPROM_readAnything(i*145,curRecipie);
    Serial.print(curRecipie.name);
    lcd.print(curRecipie.name);
  }*/
  
 /* for (int i = 0; i < 4096; i++)
    EEPROM.write(i, 0);
    
  // turn the LED on when we're done
  digitalWrite(13, HIGH);*/
}

//////////
//MASH FUNCTIONS
int getNumberOfMashSteps()
{
  return curRecipie.numberOfMashSteps; 
}

int getCurrentMashStep()
{
  return currentMashStep; 
}

unsigned int getCurrentMashTime()
{
 return curRecipie.mashTimes[currentMashStep]; 
}

float getCurrentMashTemp()
{
  return curRecipie.mashTemps[currentMashStep];
}

boolean moveToNextMashStep()
{
 if(currentMashStep == curRecipie.numberOfMashSteps-1)
  {
   currentMashStep++;
   return false;
  }else
  {
   currentMashStep++;
   return true; 
  }
}

float getMashAmmount(int i)
{
 return curRecipie.mashAmmounts[i]; 
}

void resetValveCounters()
{
  thisValveSec = 0;
  lastValveSec= 0;
  valveOpenSecs = 0;
}

//function for Mash valve function
boolean mashValveTimeDone()
{  
  //TODO!!! convert to float calculations for exact times per quart
  if(valveOpenSecs  <= (curRecipie.mashAmmounts[currentMashStep] * secondsPerQuart))
  {   
  thisValveSec = (millis()/1000);
  if(thisValveSec > lastValveSec)//if second has passed...display
    {
      lastValveSec = thisValveSec;//increment last
      valveOpenSecs++;
    }
    return false;
  }else
  {
  return true;  
  }
}

//general valve timer
boolean valveTimeDone(int totalSecs)
{
    if(valveOpenSecs <= totalSecs)
  {   
  thisValveSec = (millis()/1000);
  if(thisValveSec > lastValveSec)//if second has passed...display
    {
      lastValveSec = thisValveSec;//increment last
      valveOpenSecs++;
    }
    return false;
  }else
  {
  return true;  
  }
}

boolean motorIsOn()
{
 return curRecipie.mashMotorStates[currentMashStep];
}

float getCurrentMashTemp(int i)
{
  return curRecipie.mashTemps[i];
}


//WORT FUNCTIONS//
float wortGoalTemp()
{
  return curRecipie.wortTemp; 
}

unsigned int wortTotalTime()
{
 return curRecipie.wortTotalSecs; 
}

int currentHopsStep()
{
 return currentHopStep; 
}

int totalHopsSteps()
{
 return  curRecipie.numOfHopSteps;
}

unsigned int currentIntervalTime()
{
 return  curRecipie.hopAdditionIntervals[currentHopStep-1];
}

unsigned int hopInterval(int i)
{
 return curRecipie.hopAdditionIntervals[i]; 
}

boolean isTimeForHops()
{  
  if(lastSeco<getElapsed())
  {
    waitFlag = false;
  }
  
  int i;
  for(i=0; i<totalHopsSteps(); i++)
  {
   if(curRecipie.hopAdditionIntervals[i]==getElapsed() && !waitFlag)
   {
    lastSeco = getElapsed();
    waitFlag = true;
    //pour them hops in!
    currentHopStep++;
    return true;
   }
  } 
  return false;
}

void displayRecipieDebug()
{
   Serial.print("numberOfMashSteps:");
   Serial.print(curRecipie.numberOfMashSteps);
   Serial.println();
   
   Serial.print("numberOfHopAdditions");
   Serial.print(curRecipie.numOfHopSteps);
   Serial.println();
   
   Serial.print("MashTemps:");
   for(int i=0; i<=curRecipie.numberOfMashSteps; i++)
   {
     Serial.print(curRecipie.mashTemps[i]);
     Serial.print(" ");
   }
   Serial.println();
   
   Serial.print("MashMotorStates:");
   for(int i=0; i<=curRecipie.numberOfMashSteps; i++)
   {
     Serial.print(curRecipie.mashMotorStates[i]);
     Serial.print(" ");
   }
   Serial.println();
   
   Serial.print("mashAmmounts:");
   for(int i=0; i<=curRecipie.numberOfMashSteps; i++)
   {
     Serial.print(curRecipie.mashAmmounts[i]);
     Serial.print(" ");
   }
   Serial.println();
   
   Serial.print("mashTimes:");
   for(int i=0; i<=curRecipie.numberOfMashSteps; i++)
   {
     Serial.print(curRecipie.mashTimes[i]);
     Serial.print(" ");
   }
   Serial.println();
   
   Serial.print("WortSeconds");
   Serial.print(curRecipie.wortTotalSecs);
   Serial.println();
   
   Serial.print("WortTemp");
   Serial.print(curRecipie.wortTemp);
   Serial.println();
   
   Serial.print("numberOfHopSteps");
   Serial.print(curRecipie.numOfHopSteps);
   Serial.println();
   
   
   Serial.print("Hop Times:");
   for(int i=0; i<curRecipie.numOfHopSteps; i++)
   {
     Serial.print(curRecipie.hopAdditionIntervals[i]);
     Serial.print(" ");
   }
   Serial.println();
  
}


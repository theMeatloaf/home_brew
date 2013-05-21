
//mash variables
#define maxMashes 10

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
 //last mash step temp is sparge conditions....motor off and 
 int numberOfMashSteps;
 int numberOfHopAdditions;
 float mashTemps[maxMashes];
 boolean mashMotorStates[maxMashes];
 float mashAmmounts[maxMashes];
//needd mash times
 unsigned int spargeTime;
 unsigned int wortTotalSecs;
 unsigned int hopAdditionIntervals[3];
 float wortTemp;
 int numOfHopSteps;
};

//this recipie
static recipie curRecipie;

void setCurrentRecipie(struct recipie inputRecipie)
{
 curRecipie = inputRecipie; 
}

void setupTestRecipie()
{
curRecipie.numberOfMashSteps = 4;
curRecipie.numberOfHopAdditions = 3;
curRecipie.mashTemps[0] = 60;
curRecipie.mashTemps[1] = 80;
curRecipie.mashTemps[2] = 90;
curRecipie.mashTemps[3] = 69;
curRecipie.mashMotorStates[0] = false;
curRecipie.mashMotorStates[1] = false;
curRecipie.mashMotorStates[2] = true;
curRecipie.mashMotorStates[3] = false;
curRecipie.mashAmmounts[0] = 5;
curRecipie.mashAmmounts[1] = 5;
curRecipie.mashAmmounts[2] = 5;
curRecipie.mashAmmounts[3] = 7;
curRecipie.spargeTime = 8;
curRecipie.wortTotalSecs = 3600;
curRecipie.hopAdditionIntervals[0] = 10;
curRecipie.hopAdditionIntervals[1] = 20;
curRecipie.hopAdditionIntervals[2] = 30;
curRecipie.wortTemp = 100;
curRecipie.numOfHopSteps = 3;
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

float getCurrentMashTemp()
{
  return curRecipie.mashTemps[currentMashStep];
}

boolean moveToNextMashStep()
{
 if(currentMashStep == curRecipie.numberOfMashSteps-1)
  {
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
  if(valveOpenSecs < curRecipie.mashAmmounts[currentMashStep])
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

unsigned int getSpargeTime()
{
 return curRecipie.spargeTime; 
}

//general valve timer
boolean valveTimeDone(int totalSecs)
{
    if(valveOpenSecs < totalSecs)
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

boolean isTimeForHops()
{  
  if(lastSeco<getElapsed())
  {
    waitFlag = false;
  }
  
  int i;
  for(i=0; i<3; i++)
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




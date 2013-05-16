//temporary hard coded recipie values

//mash variables
#define maxMashes 10

//first mash step is strike conditions....so temp and ammount to be poured in...set motor OFF always!
//last mash step temp is sparge conditions....motor off and 
static int numberOfMashSteps = 4;
static int numberOfHopAdditions = 3;
static int currentMashStep = 0;
static float mashTemps[maxMashes] = {90,80,90,69};
static boolean mashMotorStates[maxMashes] = {false,false,true,false};
static float mashAmmounts[maxMashes] = {5,5,5,7};
//mash times

//vavle time vars 
static unsigned int valveOpenSecs = 0;
static unsigned int thisValveSec = 0;
static unsigned int lastValveSec = 0;

//wort variables
static unsigned int wortTotalSecs = 3600;
static unsigned int hopAdditionIntervals[3]= {10,20,30};
static float wortTemp = 100;
static int currentHopStep = 1;
static int numOfHopSteps = 3;
static boolean waitFlag = false;
static int lastSeco = 0;


//////////
//MASH FUNCTIONS
int getNumberOfMashSteps()
{
  return numberOfMashSteps; 
}

int getCurrentMashStep()
{
  return currentMashStep; 
}

float getCurrentMashTemp()
{
  return mashTemps[currentMashStep];
}

boolean moveToNextMashStep()
{
 if(currentMashStep == numberOfMashSteps-1)
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
 return mashAmmounts[i]; 
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
  if(valveOpenSecs < mashAmmounts[currentMashStep])
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
 return mashMotorStates[currentMashStep];
}

float getCurrentMashTemp(int i)
{
  return mashTemps[i];
}


//WORT FUNCTIONS//
float wortGoalTemp()
{
  return wortTemp; 
}

int currentHopsStep()
{
 return currentHopStep; 
}

int totalHopsSteps()
{
 return  numOfHopSteps;
}

unsigned int currentIntervalTime()
{
 return  hopAdditionIntervals[currentHopStep-1];
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
   if(hopAdditionIntervals[i]==getElapsed() && !waitFlag)
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


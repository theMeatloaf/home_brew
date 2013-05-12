//temporary hard coded recipie values

//mash variables
static int numberOfMashSteps = 4;
static int numberOfHopAdditions = 3;
static int currentMashStep = 1;
static double mashTemps[20];
static boolean mashMotorStates[20];

//wort variables
static unsigned int wortTotalSecs = 3600;
static unsigned int hopAdditionIntervals[3]= {10,20,30};
static double wortTemp;
static int currentHopStep = 1;
static int numOfHopSteps = 3;

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



boolean moveToNextMashStep()
{
 if(currentMashStep == numberOfMashSteps)
  {
   return false;
  }else
  {
   currentMashStep++;
   return true; 
  }
}


boolean motorIsOn()
{
 return mashMotorStates[currentMashStep];
}

double getCurrentMashTemp(int i)
{
  return mashTemps[i];
}


//WORT FUNCTIONS//
double wortGoalTemp()
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
  int i;
  for(i=0; i<3; i++)
  {
   if(hopAdditionIntervals[i]==getElapsed())
   {
    //pour them hops in!
    currentHopStep++;
    return true;
   }
  } 
  return false;
}


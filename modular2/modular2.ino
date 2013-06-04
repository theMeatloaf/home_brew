#include <DallasTemperature.h>

//define brew cases...arbitrary
extern const int mash = 1;
extern const int wort = 2;
extern const int strike = 0;
extern const int sparge = 3;
extern const int mashout = 4;
extern const int emergencyShutoff = 5;
extern const int complete = 6;

#define MASHOUT_TIME 1
#define SPARGE_WAIT_TIME 1

//most important flag!
extern boolean readyToBrew = false;

//inport global Vars
extern const int outlet1,outlet2,spargeVesselTemp, mashVesselTemp,wortVesselTemp, mash, wort, strike, motorRelay;

unsigned int last = 0;
static int brewStage = strike;
static int displayOption = 0;
static int currentOutlet;

void setup()
{
  Serial.begin(9600);

  setupPins();
  setupLCD();
  
  //init input Screens
  populateScreenVars();

  //display start screen
  displayBootScreen();

  currentOutlet = outlet1;
  setupTempController(); 

  //recipie for Testing
  //setupTestRecipie(); while(1){}

  turnOnMultiHold();
  while(!readyToBrew)
  {
    //say in here unless its really ready to brew!
    inputRecipieLoop();
  }
  turnOffMultiHold();
  
  displayRecipieDebug();

  //start Brew
  changeScreens();
  SetupHoldTemp(currentOutlet,getCurrentMashTemp(),4294967294);
}


void loop()
{ 
  if(resetScreen())
  {
   setupLCD(); 
  }
  
  if(emergencyShutdown())
  {
   //need to shutdown!
   turnAlarmOn();
    delay(500);
    turnAlarmOff();
   brewStage = emergencyShutoff;
   last = 0;
  }
  
  //check for input here and change display case acordingly...also check for emergency
  int ButtonValue = Buttonloop();
  if(ButtonValue==3 || ButtonValue==4)
  {   
    changeScreens();
    last = 0;

    if(ButtonValue == 3)displayOption--;
    else if(ButtonValue==4)
      displayOption++;

    //wrap around logic
    if(displayOption==4)
      displayOption = 0;
    if(displayOption==-1)
      displayOption = 3;
  }

  switch(brewStage)
  {
  case strike:
    {
      strikeCase();
      break;
    }

  case mash:
    {
      mashCase();
      break;
    }

  case mashout:
    {
      mashoutCase();
      break;
    }

  case sparge:
    {
      spargeCase();
      break; 
    }

  case wort:
    {
       wortCase();
       break;
    }
   case emergencyShutoff:
   {
      emergencyShutdownCase();
      break; 
   }
   case complete:
   {
     
     break; 
   }
}

  if(getAllElapsed()>last)
  {
    //A second Has gone By...decide what to display
    if(getAllElapsed() % 5 == 0) //for printing to csv format!
    {
          Serial.print(getAllElapsed());
          Serial.print(",");
          Serial.print(getTempF(getTempNew(wortVesselTemp)));
          Serial.println();
    }
    switch(displayOption)
    {
    case 0:
      displaySpargeLCD();
      break;

    case 1:
      displayMashLCD();
      break;

    case 2:
      displayWortLCD();
      break;

    case 3:
      displayOverallLCD();
      break;
    }
    last = getAllElapsed();
  }
}

void spargeCase()
{
  forceHoldTemp(spargeVesselTemp);
  digitalWrite(outlet1,LOW);//turn off in case water gets too low

  //check for when valve is done and just close spargevalve
  if(mashValveTimeDone())
  {
    closeSpargeValve();
  }
  
  //check if its been closed for a while (10 mins) before turning on wort
  if(valveTimeDone((getMashAmmount(getCurrentMashStep()) * 9.825) + SPARGE_WAIT_TIME)) //600
  {
    currentOutlet = outlet2;
    SetupHoldTemp(currentOutlet,wortGoalTemp(),wortTotalTime());
    brewStage = wort;
  }
   
}

void setupNextMashCase()
{
  closeSpargeValve(); 

  //setup next mashStep temp control
  SetupHoldTemp(currentOutlet,getCurrentMashTemp(),getCurrentMashTime()); 

  //turn on/off motor
  if(motorIsOn())
  {
    digitalWrite(motorRelay,HIGH);
  }
  else
  {
    digitalWrite(motorRelay,LOW); 
  }
  last = 0; 
}

void mashCase()
{
  if(mashValveIsOpen())
  {
      if(valveTimeDone(15))closeMashValve(); //close mashValve that was opened temporatily to allow stuff to flow!  
  }
  
  //check if valve is currently open and either just keep holding temp and keep open, or close and go to next mash step
  if(spargeValveIsOpen())
    {
      //keep holding temp
      forceHoldTemp(spargeVesselTemp);
      
      if(mashValveTimeDone())
      {
          if(moveToNextMashStep())
          {
             setupNextMashCase();
          }else
          {
             setupNextMashCase(); //setupSpargeConditions
             //move To Mashout
             resetValveCounters();
             openMashValve();
             brewStage = mashout;
             last = 0;
           }
      }
    }
  else if(HoldTempDone(spargeVesselTemp))
    {
      //need check if should open valve
       if(getMashAmmount(getCurrentMashStep())>0)
       {
          Serial.println("TIME THAT VALVE IS OPENING FOR:");
          Serial.print(getMashAmmount(getCurrentMashStep()) * 9.825);
          resetValveCounters();
          openMashValve();
          openSpargeValve();
          return;
      }else if(moveToNextMashStep())//if no valve Time then setup other stuff
      {
          setupNextMashCase();
      }else
      {
        //move To Mashout
        setupNextMashCase(); //setupSpargeConditions
        resetValveCounters();
        openMashValve();
        brewStage = mashout;
        last = 0;
      }
  }          
}

void mashoutCase()
{
  //keep holding temp
  forceHoldTemp(spargeVesselTemp);

  //when done just move to sparge..
  //NEED MASHOUT TIME VALUE HERE/////////////////
  if(valveTimeDone(MASHOUT_TIME)) //780
  {
    Serial.print("SPARGING FOR:");
    Serial.print(getMashAmmount(getCurrentMashStep()) * 9.825);
    resetValveCounters();
    openSpargeValve();
    brewStage = sparge;
  } 
}

void strikeCase()
{
  //check if valve is currently open and act acordinly (setup next mash or continue keeping it hang)
  if(spargeValveIsOpen())
  {
    forceHoldTemp(spargeVesselTemp);//keep holding temp and counting
    if(mashValveTimeDone())
    {
      //close valve
      closeSpargeValve();
      //setup next temp control
      moveToNextMashStep();
      if(motorIsOn()){digitalWrite(motorRelay,HIGH);}
      SetupHoldTemp(currentOutlet,getCurrentMashTemp(),getCurrentMashTime());
      brewStage = mash;
      last = 0;
      openMashValve();//to allow for some to flow in
    }
  }
  else if(getTempF(getTempNew(spargeVesselTemp)) > getHoldTemp() && getFinalSecsInt()>600)
  {
  SetupHoldTemp(currentOutlet,getCurrentMashTemp(),600);//hold for 10 mins more
  } else if(HoldTempDone(spargeVesselTemp))
  {
       //need to open valve now
      Serial.println("TIME THAT VALVE IS OPENING FOR:");
      Serial.print(getMashAmmount(getCurrentMashStep()) * 9.825);
      resetValveCounters();
      openSpargeValve(); 
  }
}

void wortCase()
{   
      if(getTempF(getTempNew(wortVesselTemp)) < getHoldTemp()-3)
      {
       // keep holding temp and dont do anything else
       forceHoldTemp(spargeVesselTemp);
       return;
      }else if(HoldTempDone(wortVesselTemp))
      {
       turnHeatersOff();
       openWortValve();////need to end here
       brewStage = complete;
       return;
      } 
      else if(isTimeForHops())
      {
        //increment times
      }
      else if(getElapsed() >  (convertToSeconds(getFinalHours(),getFinalMins(),getFinalSecs())-180))
      {
        turnHeatersOff();
        delay(10);//sandwitched in delays for surge protection
        turnOnPump();
        delay(10);
        if(getElapsed() <  (convertToSeconds(getFinalHours(),getFinalMins(),getFinalSecs())-178)){setupLCD();}
      }

    if(hopInterval(0)<=getElapsed() && totalHopsSteps()>0){turnOnHopper1();}
    if(hopInterval(1)<=getElapsed() && totalHopsSteps()>1){turnOnHopper2();}
    if(hopInterval(2)<=getElapsed() && totalHopsSteps()>2){turnOnHopper3();}
}

void emergencyShutdownCase()
{
    turnHeatersOff();
    closeSpargeValve();
    closeWortValve();
    closeMashValve();
    digitalWrite(motorRelay,LOW);

}

int currentBrewStage()
{
  return brewStage; 
}




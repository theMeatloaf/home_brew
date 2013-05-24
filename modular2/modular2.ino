#include <DallasTemperature.h>

//define brew cases...arbitrary
extern const int mash = 1;
extern const int wort = 2;
extern const int strike = 0;
extern const int sparge = 3;
extern const int mashout = 4;

//most important flag!
extern boolean readyToBrew = true;

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
  setupTestRecipie();

  turnOnMultiHold();
  while(!readyToBrew)
  {
    //say in here unless its really ready to brew!
    inputRecipieLoop();
  }
  turnOffMultiHold();

  //start Brew
  changeScreens();
  SetupHoldTemp(currentOutlet,getCurrentMashTemp(),4294967294);
}


void loop()
{ 
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
      openWortValve();
      if(HoldTempDone(wortVesselTemp)) while(1); 
      else
      {
        if(isTimeForHops())
        {
          //pour in the hops yo! 
        }
      }

      break;
    }
  }

  if(getAllElapsed()>last)
  {
    //A second Has gone By...decide what to display
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

  //check for when valse its done and then do wort ish
  if(valveTimeDone(getMashAmmount(getCurrentMashStep())))
  {
    closeMashValve();
    closeSpargeValve();
    digitalWrite(outlet1,LOW);
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
          Serial.print(getMashAmmount(getCurrentMashStep()));
          resetValveCounters();
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
  //NEED MASHOUT TIME VALUE HERE
  if(valveTimeDone(10))
  {
    Serial.print("SPARGING FOR:");
    Serial.print(getMashAmmount(getCurrentMashStep()));
    resetValveCounters();
    openSpargeValve();
    brewStage = sparge;
  } 
}

void strikeCase()
{
  HoldTempDone(spargeVesselTemp);

  //check if valve is currently open and act acordinly (setup next mash or continue keeping it hang)
  if(spargeValveIsOpen())
  {
    if(mashValveTimeDone())
    {
      //close valve
      closeSpargeValve(); 
      //setup next temp control
      moveToNextMashStep();
      SetupHoldTemp(currentOutlet,getCurrentMashTemp(),getCurrentMashTime());
      brewStage = mash;
      last = 0;
    }
  }
  else if(getTempF(getTempNew(spargeVesselTemp)) > getHoldTemp())
  {
    //need to open valve now
              Serial.println("TIME THAT VALVE IS OPENING FOR:");
          Serial.print(getMashAmmount(getCurrentMashStep()));
    resetValveCounters();
    openSpargeValve();
  } 
}

int currentBrewStage()
{
  return brewStage; 
}




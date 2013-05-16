#include <DallasTemperature.h>

//define brew cases...arbitrary
extern const int mash = 1;
extern const int wort = 2;
extern const int strike = 0;
extern const int sparge = 3;
extern const int mashout = 4;


//inport global Vars
extern const int outlet1,outlet2,spargeVesselTemp, mashVesselTemp,wortVesselTemp, mash, wort, strike, motorRelay;

unsigned int last = 0;
static int brewStage = strike;
static int displayOption = 0;
static int currentOutlet;

void setup()
{
 setupPins();
 setupLCD();
 
 //display start screen
 displayBootScreen();
 
 currentOutlet = outlet1;
 setupTempController(); 
 
 int ButtonValue;
 while(ButtonValue != 1)
 {
  ButtonValue = Buttonloop();
 }
 changeScreens();
 
 //start Brew
 SetupHoldTemp(currentOutlet,getCurrentMashTemp(),1,0,10);
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
           HoldTempDone(spargeVesselTemp);

           //check if valve is currently open and act acordinly (setup next mash or continue keeping it hang)
           if(spargeValveIsOpen())
           {
                 if(mashValveTimeDone())
                  {
                  //close valve
                  closeSpargeValve(); 
                  brewStage = mash;
                  last = 0;
                  }
           }else if(getTempF(getTempNew(spargeVesselTemp)) > getHoldTemp())
           {
               //need to open valve now
               resetValveCounters();
               openSpargeValve();
               //setup next temp control
               moveToNextMashStep();
               SetupHoldTemp(currentOutlet,getCurrentMashTemp(),10);
           }
           break;
       }
    
       case mash:
       {
          //check if valve is currently open and either just keep holding temp and keep open, or close and go to next mash step
          if(spargeValveIsOpen())
           {
               //keep holding temp
                forceHoldTemp(spargeVesselTemp);
                if(mashValveTimeDone())
                 {
                   closeSpargeValve(); 
                   
                   //setup next mashStep temp control
                   SetupHoldTemp(currentOutlet,getCurrentMashTemp(),10); 
                   
                    //turn on/off motor
                    if(motorIsOn())
                    {
                      digitalWrite(motorRelay,HIGH);
                    }else
                    {
                      digitalWrite(motorRelay,LOW); 
                    }
                    last = 0;
                 }
           }else if(HoldTempDone(spargeVesselTemp))
           {
              if(moveToNextMashStep())
              {
                //need check if should open valve
                if(getMashAmmount(getCurrentMashStep())>0)
                {
                   resetValveCounters();
                   openSpargeValve();
                   break;
                }else
                {
                  //setup next mashStep temp control
                     SetupHoldTemp(currentOutlet,getCurrentMashTemp(),10); 
                     
                      //turn on/off motor
                      if(motorIsOn())
                      {
                        digitalWrite(motorRelay,HIGH);
                      }else
                      {
                        digitalWrite(motorRelay,LOW); 
                      }
                }
              }else
             {
              //move To Mashout
              resetValveCounters();
              openMashValve();
              brewStage = mashout;
              last = 0;
             }
           }         
         break;
       }
       
       case mashout:
       {
        //keep holding temp
        forceHoldTemp(spargeVesselTemp);
         
         //when done just move to sparge..
         //NEED MASHOUT TIME VALUE HERE
         Serial.print(getMashAmmount(getCurrentMashStep()));
         if(valveTimeDone(getMashAmmount(getCurrentMashStep())))
         {
          resetValveCounters();
          openSpargeValve();
          brewStage = sparge;
         }
        break;
       }
       
       case sparge:
       {
         forceHoldTemp(spargeVesselTemp);
         
        //check for when valse its done and then do wort ish
        //NEED SPARGE AMMOUNT HERE
        if(valveTimeDone(10))
        {
              closeMashValve();
              closeSpargeValve();
              currentOutlet = outlet2;
              SetupHoldTemp(currentOutlet,wortGoalTemp(),1000);
              brewStage = wort;
        }
        break; 
       }
       
       case wort:
       {
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
       
       case 69:
       {
         //do less for now
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

int currentBrewStage()
{
  return brewStage; 
}

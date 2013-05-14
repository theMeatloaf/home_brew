#include <DallasTemperature.h>

//define brew cases
extern const int mash = 1;
extern const int wort = 2;
extern const int strike = 0;

//inport global Vars
extern const int outlet1,outlet2,spargeVesselTemp, mashVesselTemp,wortVesselTemp, mash, wort, strike;

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
           if(getTempF(getTempNew(spargeVesselTemp)) > getHoldTemp())
           {
             //need to open valve now for time
             moveToNextMashStep();
             SetupHoldTemp(currentOutlet,getCurrentMashTemp(),10);
             brewStage = mash;
             last = 0;
           }
           break;
       }
    
       case mash:
       {
         if(HoldTempDone(spargeVesselTemp))
          {
            if(moveToNextMashStep())
            {
              //setup next mashStep
              SetupHoldTemp(currentOutlet,getCurrentMashTemp(),10);
              last = 0;
            }else
            {
              currentOutlet = outlet2;
              SetupHoldTemp(currentOutlet,wortGoalTemp(),1000);
              brewStage=wort;
              last = 0;
            }
          }
         break;
       }
       case wort:
       {
        if(HoldTempDone(wortVesselTemp)) while(1); 
        
        if(isTimeForHops())
        {
               //pour in the hops yo! 
        }
        break;
       }
       
       case 69:
       {
         //do less for now
       }
  }
     
 if(getElapsed()>last)
 {
    //A second Has gone By...decide what to display
    switch(displayOption)
    {
      case 0:
        displayTimeAndTemp();
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
    last = getElapsed();
 }
}

int currentBrewStage()
{
  return brewStage; 
}

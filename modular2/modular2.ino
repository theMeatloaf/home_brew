#include <DallasTemperature.h>


//define brew cases
#define mash 0
#define wort 1



//inport global Vars
extern const int outlet1;

unsigned int last = 0;
static int brewStage = 0;
static int displayOption = 0;
static int currentOutlet;

void setup()
{
 currentOutlet = outlet1;
 setupPins();
 setupLCD();
 setupTempController(); 
 SetupHoldTemp(currentOutlet,88,0,0,10);
}


void loop()
{ 
  //check for input here and change display case acordingly...also check for emergency
  int ButtonValue = Buttonloop();
 if(ButtonValue==3 || ButtonValue==4)
 {
    changeScreens();
    if(displayOption==2)
    displayOption=0;
    else
    displayOption++;
 }
  switch(brewStage)
  {
       case mash:
       {
         if(HoldTempDone(0))
          {
            if(moveToNextMashStep())
            {
              //setup next mashStep
              SetupHoldTemp(currentOutlet,98.5,10);
              last = 0;
            }else
            {
              brewStage=wort; 
            }
          }
         break;
       }
       case wort:
       {
        if(HoldTempDone(0)) while(1); 
        
        if(isTimeForHops())
        {
               //pour in the hops yo! 
        }
       }
       
       case 2:
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
    }
    last = getElapsed();
 }
}

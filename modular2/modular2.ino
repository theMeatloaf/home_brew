
//inport Pins
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
    if(displayOption==0)
    displayOption=1;
    else
    displayOption=0;
 }
  switch(brewStage)
  {
       case 0:
       {
         if(HoldTempDone())
          {
            brewStage++;
          }
         break;
       }
       case 1:
       {
         //displayOption++;
         SetupHoldTemp(currentOutlet,100,1,5,22);
         last = 0;
         brewStage++;
         break;
       }
       
       case 2:
       {
        if(HoldTempDone()) while(1); 
       }
  }
     
 if(getElapsed()>last)
 {
    //A second Has gone By...decide what to display
    switch(displayOption)
    {
      case 0:
        displayTimeAndTemp();
        displayTimeAndTempLCD();
        break;
        
      case 1:
        displayElapsSecsLCD();
        Serial.println(getElapsed()); 
        break;
    }
    last = getElapsed();
 }
}

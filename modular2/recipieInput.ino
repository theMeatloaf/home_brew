#define maxScreenValues 10

#define mashInputScreen 7
#define spargeQuestionScreen 8
#define wortInputScreen 9

#define integerVar 1
#define floatVar 2
#define on_offVar 3
#define yes_noVar 4
#define swapBool 5
#define doneVar 6

#define NUM_OF_MASH_INPUTS 7
#define NUM_OF_SPARGE_INPUTS 2

#define FLASH_MILLIS 500

extern boolean readyToBrew;

//ints for flashing!
unsigned int lastFlashInt = millis()/FLASH_MILLIS;
unsigned int thisFlashInt;
int i;

//recipie struct to populate
static recipie inputRecipie;

//recipie temp vars
static unsigned int tempMashHours,tempMashMins,tempMashSecs;
static int tempMashIntervals;
static float tempMashTemp;
static boolean tempMashMotorOn,tempMoreMashSteps;

//screen struct
struct screen
{
  int id;
  int locationRows[maxScreenValues];
  int locationCollums[maxScreenValues];
  //what each var type is....1 is int, 2 is float, 3 is off/on, 4 is yes/no, 5 is swap bool, 6 is DONE
  int varTypes[maxScreenValues];
  float * floatVars[maxScreenValues];
  unsigned int * intVars[maxScreenValues];
  boolean * onOffVars[maxScreenValues];
  boolean * yesNoVars[maxScreenValues];
  int varWidths[maxScreenValues];
};

//screen and edit counters
static screen currentScreen;
static int curEdit=0;

//screen defs
screen mashScreen, spargeQuestScreen;

//mash hardcoded loactions
int mashRowLocations[NUM_OF_MASH_INPUTS] = {9,5,8,11,6,6,10};
int mashCollumnLocations[NUM_OF_MASH_INPUTS] = {0,1,1,1,2,3,3};
int mashVarTypes[NUM_OF_MASH_INPUTS] = {2,1,1,1,3,4,6};
int mashVarWidths[NUM_OF_MASH_INPUTS] = {6,2,2,2,3,3,5};
float * mashVarFloatVars[NUM_OF_MASH_INPUTS] = {&tempMashTemp,0,0,0,0,0,0};
unsigned int * mashVarIntVars[NUM_OF_MASH_INPUTS] = {0, &tempMashHours,&tempMashMins,&tempMashSecs,0,0,0};
boolean * mashOnOffVars[NUM_OF_MASH_INPUTS] = {0,0,0,0,&tempMashMotorOn,0,0};
boolean * mashYesNoVars[NUM_OF_MASH_INPUTS] = {0,0,0,0,0,&tempMoreMashSteps,0};


//sparge Q screen locations
int spargeRowsLocations[NUM_OF_SPARGE_INPUTS] = {6, 8};
int spargeCollumnLocations[NUM_OF_SPARGE_INPUTS] = {1,3};

void populateScreenVars()
{   
  //mash defenitons
  mashScreen.id = mashInputScreen;
  for(i=0; i<NUM_OF_MASH_INPUTS; i++)
  {
     mashScreen.locationRows[i] = mashRowLocations[i];
     mashScreen.locationCollums[i] = mashCollumnLocations[i];
     mashScreen.varTypes[i] = mashVarTypes[i];
     mashScreen.varWidths[i] = mashVarWidths[i];
     mashScreen.floatVars[i] = mashVarFloatVars[i];
     mashScreen.intVars[i] = mashVarIntVars[i];
     mashScreen.onOffVars[i] = mashOnOffVars[i];
     mashScreen.yesNoVars[i] = mashYesNoVars[i];
  }
  
  
 /* for(i=0; i<NUM_OF_SPARGE_INPUTS; i++)
  {
    spargeQuestScreen.locationRows[i] = spargeRowsLocations[i];
    spargeQuestScreen.locationCollums[i] = spargeCollumnLocations[i];
  }*/

  //initialize currentScreen
  currentScreen = mashScreen;
}


static boolean flashFlag = false;

void inputRecipieLoop()
{
  int ButtonValue = ButtonloopRepeate();
  if(ButtonValue != -1)
  {
    //code to refresh screen quickly
    thisFlashInt--;
    flashFlag=false;
  }
  switch(ButtonValue)
  { 
      case 2:
      {
       //down
       decreaseSelection();
       break;
      } 
      
      case 1:
      {
       //up
       increaseSelection();
       break;
      }
      
      case 3:
      {
        //left
        moveSelectionLeft();
        break;
      }
      
      case 4:
      {
        //right
        moveSelectionRight();
        break;
      }
  
  }
    
  lcd.setCursor(currentRow(),currentCollumn());
  if(flashFlag)
  {
     for(i=0; i<curLength(); i++)
     {
      lcd.print(" ");
     }
  }else
  {
       printCurEditValue();
  }
  
  
  if(thisFlashInt>lastFlashInt)
  {
    printCurInputScreen();
    flashFlag = !flashFlag;
    lastFlashInt = thisFlashInt;
  }
  thisFlashInt = millis()/FLASH_MILLIS;

}

void printCurEditValue()
{ 
     if(currentVarType() == floatVar)
     {
      lcd.print(*currentScreen.floatVars[curEdit]);
     }else if(currentVarType() == integerVar)
     {
      for(i=0; i<curLength(); i++)
      {
       if(*currentScreen.intVars[curEdit]<(i*10))lcd.print("0"); 
      }
      lcd.print(*currentScreen.intVars[curEdit]); 
     } 
}

int currentVarType()
{
     return currentScreen.varTypes[curEdit];  
}

void printCurInputScreen()
{   
        if(currentScreen.id == mashInputScreen)
        {
           lcd.setCursor(0,0);
           lcd.print("#");
           if(inputRecipie.numberOfMashSteps < 10)lcd.print("0");
           lcd.print(inputRecipie.numberOfMashSteps);
           lcd.print(" Temp:");
           lcd.print(tempMashTemp);
           lcd.setCursor(0,1);
           lcd.print("TIME:");
           if(tempMashHours<10)lcd.print("0");
           lcd.print(tempMashHours);
           lcd.print(":");
           if(tempMashMins<10)lcd.print("0");
           lcd.print(tempMashMins);
           lcd.print(":");
           if(tempMashSecs<10)lcd.print("0");
           lcd.print(tempMashSecs);
           lcd.setCursor(0,2);
           lcd.print("Motor:");
           if(tempMashMotorOn)lcd.print("ON ");
           else lcd.print("OFF");
           lcd.setCursor(0,3);
           lcd.print("More?:");
           if(tempMoreMashSteps)lcd.print("YES");
           else lcd.print("NO ");
           lcd.print(" DONE?");
         } 
}

int currentRow()
{
    return currentScreen.locationRows[curEdit]; 
}

int currentCollumn()
{
    return currentScreen.locationCollums[curEdit];
}

int curLength()
{
      return currentScreen.varWidths[curEdit];
}

void moveSelectionRight()
{
  curEdit++;
}

void moveSelectionLeft()
{
  curEdit--;
}

void increaseSelection()
{  
       if(currentVarType() == floatVar)
       {
         *currentScreen.floatVars[curEdit] = *currentScreen.floatVars[curEdit]+0.5;
       }
       if(currentVarType() == integerVar)
       {
         *currentScreen.intVars[curEdit] = *currentScreen.intVars[curEdit]+1;
       }
       if(currentVarType() == on_offVar)
       {
          *currentScreen.onOffVars[curEdit] = !*currentScreen.onOffVars[curEdit];
       }
       if(currentVarType() == yes_noVar)
       {
         *currentScreen.yesNoVars[curEdit] = !*currentScreen.yesNoVars[curEdit];
       }
       if(currentVarType() == doneVar)
       {
        //need to go to next Screen
        Serial.print("NEXT SCREEN!"); 
       }
}

void decreaseSelection()
{
       if(currentVarType() == floatVar && *currentScreen.floatVars[curEdit]>0.5)
       {
         *currentScreen.floatVars[curEdit] = *currentScreen.floatVars[curEdit]-0.5;
       }
       if(currentVarType() == integerVar && *currentScreen.intVars[curEdit]>0)
       {
         *currentScreen.intVars[curEdit] = *currentScreen.intVars[curEdit]-1;
       }
       if(currentVarType() == on_offVar)
       {
          *currentScreen.onOffVars[curEdit] = !*currentScreen.onOffVars[curEdit];
       }
       if(currentVarType() == yes_noVar)
       {
         *currentScreen.yesNoVars[curEdit] = !*currentScreen.yesNoVars[curEdit];
       }
}



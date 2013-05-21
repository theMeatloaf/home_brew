#define maxScreenValues 10

#define mashInputScreen 7
#define spargeQuestionScreen 8
#define wortInputScreen 9

#define integerVar 1
#define floatVar 2
#define on/offVAr 3
#define yes/noVar 4
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

//screen and edit counters
static int currentScreen = mashInputScreen;
static int curEdit=0;

//recipie struct to populate
static recipie inputRecipie;

//recipie temp vars
static unsigned int tempMashHours,tempMashMins,tempMashSecs;
static int tempMashIntervals;
static float tempMashTemp;

//screen struct
struct screen
{
  int numOfInputs;
  int locationRows[maxScreenValues];
  int locationCollums[maxScreenValues];
  //what each var type is....1 is int, 2 is float, 3 is off/on, 4 is yes/no, 5 is swap bool, 6 is DONE
  int varTypes[maxScreenValues];
  float * floatVars[maxScreenValues];
  unsigned int * intVars[maxScreenValues];
  //float incAmmounts[];
  int varWidths[maxScreenValues];
};

//screen defs
screen mashScreen, spargeQuestScreen;

//mash hardcoded loactions
int mashRowLocations[NUM_OF_MASH_INPUTS] = {9,5,8,11,6,6,10};
int mashCollumnLocations[NUM_OF_MASH_INPUTS] = {0,1,1,1,2,3,3};
int mashVarTypes[NUM_OF_MASH_INPUTS] = {2,1,1,1,3,4,6};
int mashVarWidths[NUM_OF_MASH_INPUTS] = {6,2,2,2,3,3,3};
float * mashVarFloatVars[NUM_OF_MASH_INPUTS] = {&tempMashTemp,0,0,0,0,0,0};
unsigned int * mashVarIntVars[NUM_OF_MASH_INPUTS] = {0, &tempMashHours,&tempMashMins,&tempMashSecs,0,0,0};


//sparge Q screen locations
int spargeRowsLocations[NUM_OF_SPARGE_INPUTS] = {6, 8};
int spargeCollumnLocations[NUM_OF_SPARGE_INPUTS] = {1,3};

void populateScreenVars()
{   
  //mash defenitons
  for(i=0; i<NUM_OF_MASH_INPUTS; i++)
  {
     mashScreen.locationRows[i] = mashRowLocations[i];
     mashScreen.locationCollums[i] = mashCollumnLocations[i];
     mashScreen.varTypes[i] = mashVarTypes[i];
     mashScreen.varWidths[i] = mashVarWidths[i];
     mashScreen.floatVars[i] = mashVarFloatVars[i];
     mashScreen.intVars[i] = mashVarIntVars[i];
  }
  
  
 /* for(i=0; i<NUM_OF_SPARGE_INPUTS; i++)
  {
    spargeQuestScreen.locationRows[i] = spargeRowsLocations[i];
    spargeQuestScreen.locationCollums[i] = spargeCollumnLocations[i];
  }*/
}


static boolean flashFlag = false;

void inputRecipieLoop()
{
  int ButtonValue = Buttonloop();
  if(ButtonValue != -1)lastFlashInt=0;
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
  
  
  thisFlashInt = millis()/FLASH_MILLIS;
  if(thisFlashInt>lastFlashInt)
  {
      printCurInputScreen();

    flashFlag = !flashFlag;
    lastFlashInt = thisFlashInt;
  }
}

void printCurEditValue()
{
  switch(currentScreen)
  {
   case mashInputScreen:
   {
     if(currentVarType() == floatVar)
     {
      lcd.print(*mashScreen.floatVars[curEdit]);
     }else if(currentVarType() == integerVar)
     {
      for(i=0; i<curLength(); i++)
      {
       if(*mashScreen.intVars[curEdit]<(i*10))lcd.print("0"); 
      }
      lcd.print(*mashScreen.intVars[curEdit]); 
     }
   } 
  }
}

int currentVarType()
{
   switch(currentScreen)
  {
   case mashInputScreen:
   {
     return mashScreen.varTypes[curEdit]; 
   }
   //cases////
  } 
}

void printCurInputScreen()
{   
   switch(currentScreen)
     {
        case mashInputScreen:
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
           
           break;
        } 
     }
}

int currentRow()
{
   switch(currentScreen)
   {  
     case mashInputScreen: return mashScreen.locationRows[curEdit]; 
     case spargeQuestionScreen: return spargeQuestScreen.locationRows[curEdit];
   } 
}

int currentCollumn()
{
  switch(currentScreen)
  {
   case mashInputScreen: return mashScreen.locationCollums[curEdit];
   case spargeQuestionScreen: return spargeQuestScreen.locationCollums[curEdit];
  } 
}

int curLength()
{
  switch(currentScreen)
 {
   case mashInputScreen: 
   {
      return mashScreen.varWidths[curEdit];
   }
 } 
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
     *mashScreen.floatVars[curEdit] = *mashScreen.floatVars[curEdit]+0.5;
   }
   if(currentVarType() == integerVar)
   {
     *mashScreen.intVars[curEdit] = *mashScreen.intVars[curEdit]+1;
   }
}

void decreaseSelection()
{
  switch(curEdit)
  {
    case 0:
    {
     break;
    }
    
    case 1:
    { 
     break;
    }
  }
}

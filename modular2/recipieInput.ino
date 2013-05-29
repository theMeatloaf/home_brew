//TODO: GET RID OF TEMP AND AMNT DURING MASH INPUT....FIGURE OUT LAST VS SPARGE AND YES/NO

#define maxScreenValues 14

#define mashInputScreen 7
#define spargeQuestionScreen 8
#define wortInputScreen 9
#define strikeInputScreen 10
#define spargeDataInputScreen 11
#define wortInputScreen 12
#define saveStartQuestionScreen 13

#define integerVar 1
#define floatVar 2
#define on_offVar 3
#define yes_noVar 4
#define swapBool 5
#define doneVar 6

#define NUM_OF_MASH_INPUTS 8
#define NUM_OF_SPARGE_INPUTS 2
#define NUM_OF_STRIKE_INPUTS 3
#define NUM_OF_WORT_INPUTS 14
#define NUM_OF_SAVE_START_INPUTS 2

#define FLASH_MILLIS 400

extern boolean readyToBrew;

//ints for flashing!
unsigned int lastFlashInt = millis()/FLASH_MILLIS;
unsigned int thisFlashInt;
int i;

//recipie struct to populate
static recipie inputRecipie;

//recipie temp vars
static int tempMashHours,tempMashMins,tempMashSecs;
static int tempMashIntervals;
static float tempMashTemp,tempMashAmmount;
static float tempWortTemp = 200;
static boolean tempMashMotorOn,tempMoreMashSteps, tempHasSparge, tempSave;
static int hopIntHours[3],hopIntMins[3],hopIntSecs[3];

//screen struct
struct screen
{
  int id;
  int locationRows[maxScreenValues];
  int locationCollums[maxScreenValues];
  //what each var type is....1 is int, 2 is float, 3 is off/on, 4 is yes/no, 5 is swap bool, 6 is DONE
  int varTypes[maxScreenValues];
  float * floatVars[maxScreenValues];
  int * intVars[maxScreenValues];
  boolean * onOffVars[maxScreenValues];
  boolean * yesNoVars[maxScreenValues];
  int varWidths[maxScreenValues];
};

//screen and edit counters
static screen currentScreen;
static int curEdit=0;
static int curDisplayedMashStep = 0;

//screen defs
screen mashScreen, strikeScreen, spargeQuestScreen, spargeDataScreen, wortScreen, saveQuestionScreen;

//mash screen hardcoded vars
int mashRowLocations[NUM_OF_MASH_INPUTS] = {9,5,8,11,6,15,6,10};
int mashCollumnLocations[NUM_OF_MASH_INPUTS] = {0,1,1,1,2,2,3,3};
int mashVarTypes[NUM_OF_MASH_INPUTS] = {floatVar,integerVar,integerVar,integerVar,on_offVar,floatVar,yes_noVar,doneVar};
int mashVarWidths[NUM_OF_MASH_INPUTS] = {6,2,2,2,3,5,3,5};
float * mashVarFloatVars[NUM_OF_MASH_INPUTS] = {&tempMashTemp,0,0,0,0,&tempMashAmmount,0,0};
int * mashVarIntVars[NUM_OF_MASH_INPUTS] = {0, &tempMashHours,&tempMashMins,&tempMashSecs,0,0,0,0};
boolean * mashOnOffVars[NUM_OF_MASH_INPUTS] = {0,0,0,0,&tempMashMotorOn,0,0,0};
boolean * mashYesNoVars[NUM_OF_MASH_INPUTS] = {0,0,0,0,0,0,&tempMoreMashSteps,0};

//strike screen hardcoded vars
int strikeRowLocations[NUM_OF_STRIKE_INPUTS] = {5,8,0};
int strikeCollumnLocations[NUM_OF_STRIKE_INPUTS] = {1,2,3};
int strikeVarTypes[NUM_OF_STRIKE_INPUTS] = {floatVar,floatVar,doneVar};
int strikeVarWidths[NUM_OF_STRIKE_INPUTS] = {6,6,5};
float * strikeVarFloatVars[NUM_OF_STRIKE_INPUTS] = {&tempMashTemp,&tempMashAmmount,0};


//sparge Q screen vars
int spargeRowsLocations[NUM_OF_SPARGE_INPUTS] = {8, 7};
int spargeCollumnLocations[NUM_OF_SPARGE_INPUTS] = {2,3};
int spargeVarTypes[NUM_OF_SPARGE_INPUTS] = {yes_noVar,doneVar};
boolean * spargeYesNoVars[NUM_OF_SPARGE_INPUTS] = {&tempHasSparge, 0};
int spargeVarWidths[NUM_OF_SPARGE_INPUTS] = {3,5};

//wort screenvars
int wortRowsLocations[NUM_OF_WORT_INPUTS] = {5,12,15,18,5,8,11,5,8,11,5,8,11,14};
int wortCollumnLocations[NUM_OF_WORT_INPUTS] = {0,0,0,0,1,1,1,2,2,2,3,3,3,3};
int wortVarTypes[NUM_OF_WORT_INPUTS] = {floatVar,integerVar,integerVar,integerVar,integerVar,integerVar,integerVar,integerVar,integerVar,integerVar,integerVar,integerVar,integerVar,doneVar};
int wortVarWidths[NUM_OF_WORT_INPUTS] = {6,2,2,2,2,2,2,2,2,2,2,2,2,5};
float * wortVarFloatVars[NUM_OF_WORT_INPUTS] = {&tempWortTemp,0,0,0,0,0,0,0,0,0,0,0,0,0};
int * wortVarIntVars[NUM_OF_WORT_INPUTS] = {0,&tempMashHours,&tempMashMins,&tempMashSecs,&hopIntHours[0],&hopIntMins[0],&hopIntSecs[0],&hopIntHours[1],&hopIntMins[1],&hopIntSecs[1],&hopIntHours[2],&hopIntMins[2],&hopIntSecs[2],0};

//save/start screen vars
int saveStartRowLocations[NUM_OF_SAVE_START_INPUTS] = {12,8};
int saveStartCollumnLocaitons[NUM_OF_SAVE_START_INPUTS] = {1,2};
int saveStartVarTypes[NUM_OF_SAVE_START_INPUTS] = {yes_noVar,doneVar};
int saveStartVarWidths[NUM_OF_SAVE_START_INPUTS] = {3,6};
boolean * saveStartYesNoVars[NUM_OF_SAVE_START_INPUTS] = {&tempSave,0};

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
  
  strikeScreen.id = strikeInputScreen; 
  spargeDataScreen.id = spargeDataInputScreen;
  for(i=0; i<NUM_OF_STRIKE_INPUTS; i++)
  {
    spargeDataScreen.locationRows[i] = strikeRowLocations[i];
    spargeDataScreen.locationCollums[i] = strikeCollumnLocations[i];
    spargeDataScreen.varTypes[i] = strikeVarTypes[i];
    spargeDataScreen.varWidths[i] = strikeVarWidths[i];
    spargeDataScreen.floatVars[i] = strikeVarFloatVars[i];
    strikeScreen.locationRows[i] = strikeRowLocations[i];
    strikeScreen.locationCollums[i] = strikeCollumnLocations[i];
    strikeScreen.varTypes[i] = strikeVarTypes[i];
    strikeScreen.varWidths[i] = strikeVarWidths[i];
    strikeScreen.floatVars[i] = strikeVarFloatVars[i];
  }
  
  spargeQuestScreen.id = spargeQuestionScreen;
  for(i=0; i<NUM_OF_SPARGE_INPUTS; i++)
  {
    spargeQuestScreen.locationRows[i] = spargeRowsLocations[i];
    spargeQuestScreen.locationCollums[i] = spargeCollumnLocations[i];
    spargeQuestScreen.varTypes[i] = spargeVarTypes[i];
    spargeQuestScreen.yesNoVars[i] = spargeYesNoVars[i];
    spargeQuestScreen.varWidths[i] = spargeVarWidths[i];
  }

  wortScreen.id = wortInputScreen;
  for(i=0; i<NUM_OF_WORT_INPUTS; i++)
  {
     wortScreen.locationRows[i] = wortRowsLocations[i];
     wortScreen.locationCollums[i] = wortCollumnLocations[i];
     wortScreen.varTypes[i] = wortVarTypes[i];
     wortScreen.varWidths[i] = wortVarWidths[i];
     wortScreen.floatVars[i] = wortVarFloatVars[i];
     wortScreen.intVars[i] = wortVarIntVars[i];
  }
  
  saveQuestionScreen.id = saveStartQuestionScreen;
  for(i=0; i<NUM_OF_SAVE_START_INPUTS; i++)
  {
    saveQuestionScreen.locationRows[i] = saveStartRowLocations[i];
    saveQuestionScreen.locationCollums[i] = saveStartCollumnLocaitons[i];
    saveQuestionScreen.varTypes[i] = saveStartVarTypes[i];
    saveQuestionScreen.varWidths[i] = saveStartVarWidths[i];
    saveQuestionScreen.yesNoVars[i] =saveStartYesNoVars[i];
  }

  //initialize currentScreen
  currentScreen = strikeScreen;
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
       printCurInputScreen();
  }
  
  
  if(thisFlashInt>lastFlashInt)
  {
    flashFlag = !flashFlag;
    lastFlashInt = thisFlashInt;
  }
  thisFlashInt = millis()/FLASH_MILLIS;

}

/*void printCurEditValue()
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
}*/

int currentVarType()
{
     return currentScreen.varTypes[curEdit];  
}

void printCurInputScreen()
{   
    switch(currentScreen.id)
    {
        case mashInputScreen:
        {
           lcd.setCursor(0,0);
           lcd.print("#");
           if(curDisplayedMashStep < 10)lcd.print("0");
           lcd.print(curDisplayedMashStep);
           if(tempMoreMashSteps)
           {
               lcd.print(" Temp:");
               lcd.print(tempMashTemp);
           }else
           {
               //make it not appear
               if(curEdit==0)
               {
                curEdit++; 
               }
               lcd.print("            ");
               tempMashTemp = 0; 
           }

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
           if(tempMashMotorOn)lcd.print("ON  ");
           else lcd.print("OFF ");
           if(tempMoreMashSteps)
           {
              lcd.print("Amnt:");
              lcd.print(tempMashAmmount);
           }else
           { 
              //make it not appear
              lcd.print("          ");
              tempMashAmmount = 0; 
           }
           
           lcd.setCursor(0,3);
           lcd.print("More?:");
           if(tempMoreMashSteps)lcd.print("YES");
           else lcd.print("NO ");
           lcd.print(" DONE?");
           break;
         } 
         case strikeInputScreen:
         {
           lcd.setCursor(6,0);
           lcd.print("STRIKE:");
           lcd.setCursor(0,1);
           lcd.print("TEMP:");
           lcd.print(tempMashTemp);
           lcd.setCursor(0,2);
           lcd.print("Ammount:");
           lcd.print(tempMashAmmount);
           lcd.setCursor(0,3);
           lcd.print("DONE?");
           break;
         }
         case spargeQuestionScreen:
         {
           lcd.setCursor(6,0);
           lcd.print("SPARGE?");
           lcd.setCursor(8,2);
           if(tempHasSparge)lcd.print("YES");
           else lcd.print("NO");
           lcd.setCursor(7,3);
           lcd.print("DONE?");
           break;
         }
         case spargeDataInputScreen:
         {
           lcd.setCursor(6,0);
           lcd.print("SPARGE:");
           lcd.setCursor(0,1);
           lcd.print("TEMP:");
           lcd.print(tempMashTemp);
           lcd.setCursor(0,2);
           lcd.print("Ammount:");
           lcd.print(tempMashAmmount);
           lcd.setCursor(0,3);
           lcd.print("DONE?");
           break; 
         }
         case wortInputScreen:
         { 
          lcd.setCursor(0,0);
          lcd.print("TEMP:");
          lcd.print(tempWortTemp);
          lcd.print(" ");
          if(tempMashHours<10)lcd.print("0");
          lcd.print(tempMashHours);
          lcd.print(":");
          if(tempMashMins<10)lcd.print("0");
          lcd.print(tempMashMins);
          lcd.print(":");
          if(tempMashSecs<10)lcd.print("0");
          lcd.print(tempMashSecs);
          
          lcd.setCursor(0,1);
          lcd.print("HT1: ");
          if(hopIntHours[0]<10)lcd.print("0");
          lcd.print(hopIntHours[0]);
          lcd.print(":");
          if(hopIntMins[0]<10)lcd.print("0");
          lcd.print(hopIntMins[0]);
          lcd.print(":");
          if(hopIntSecs[0]<10)lcd.print("0");
          lcd.print(hopIntSecs[0]);
          
          lcd.setCursor(0,2);
          lcd.print("HT2: ");
          if(hopIntHours[1]<10)lcd.print("0");
          lcd.print(hopIntHours[1]);
          lcd.print(":");
          if(hopIntMins[1]<10)lcd.print("0");
          lcd.print(hopIntMins[1]);
          lcd.print(":");
          if(hopIntSecs[1]<10)lcd.print("0");
          lcd.print(hopIntSecs[1]);
          
          lcd.setCursor(0,3);
          lcd.print("HT3: ");
          if(hopIntHours[2]<10)lcd.print("0");
          lcd.print(hopIntHours[2]);
          lcd.print(":");
          if(hopIntMins[2]<10)lcd.print("0");
          lcd.print(hopIntMins[2]);
          lcd.print(":");
          if(hopIntSecs[2]<10)lcd.print("0");
          lcd.print(hopIntSecs[2]);
          
          lcd.print(" DONE?");
          break;
         }
         
         case saveStartQuestionScreen:
         {
          lcd.setCursor(3,0);
          lcd.print("RECIPIE ENTERED");
         
          lcd.setCursor(6,1);
          lcd.print("Save? ");
          
          if(tempSave)lcd.print("YES");
          else lcd.print("NO ");
          
          lcd.setCursor(8,2);
          if(tempSave)lcd.print("NEXT  ");
          else lcd.print("START!");
          
          break;
         }
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
    if(currentVarType() == doneVar)
    {
     //need to go to next Screen
        screenDone();
        return;
    }
   
  curEdit++;
  
  //custom skip logic
  if(curEdit==5 && !tempMoreMashSteps && currentScreen.id==mashInputScreen)
  {
     curEdit++; 
  } 
}

void moveSelectionLeft()
{  
  if(curEdit==0)
  {
    screenBack();
    return;
  }
  curEdit--;
  
  //custom skip logic
  if(curEdit==5 && !tempMoreMashSteps && currentScreen.id==mashInputScreen)
  {
     curEdit--; 
  }
  if(curEdit==0 && !tempMoreMashSteps && currentScreen.id==mashInputScreen)
  {
    screenBack(); 
  }
}

void increaseSelection()
{  
       if(currentVarType() == floatVar)
       {
         *currentScreen.floatVars[curEdit] = *currentScreen.floatVars[curEdit]+0.25;
       }
       if(currentVarType() == integerVar && *currentScreen.intVars[curEdit]<59)
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
}

void decreaseSelection()
{
       if(currentVarType() == floatVar && *currentScreen.floatVars[curEdit]>0.25)
       {
         *currentScreen.floatVars[curEdit] = *currentScreen.floatVars[curEdit]-0.25;
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

void screenDone()
{
 switch(currentScreen.id) 
  {
   case strikeInputScreen:
   {
       //go to Mash screen
       inputRecipie.mashTemps[0] = tempMashTemp;
       inputRecipie.mashAmmounts[0] = tempMashAmmount;
       inputRecipie.mashMotorStates[0] = false;
       inputRecipie.mashTimes[0] = 65534;//max Time
       if(inputRecipie.numberOfMashSteps==0)inputRecipie.numberOfMashSteps++;
       curDisplayedMashStep++;
       curEdit= 0;
       lcd.clear();
       tempMashTemp = 0;
       tempMashAmmount = 0;
       currentScreen = mashScreen;
       break;
   }
   case mashInputScreen:
   {
      //store current Vars
      inputRecipie.mashTemps[curDisplayedMashStep] = tempMashTemp;
      inputRecipie.mashAmmounts[curDisplayedMashStep] = tempMashAmmount;
      inputRecipie.mashMotorStates[curDisplayedMashStep] = tempMashMotorOn;
      inputRecipie.mashTimes[curDisplayedMashStep] = convertToSeconds(tempMashHours,tempMashMins,tempMashSecs);
      curEdit = 0;
      lcd.clear();
      if(tempMoreMashSteps)
       {
          if(curDisplayedMashStep<inputRecipie.numberOfMashSteps)
          {
             //fill temp vars if they already exist
             curDisplayedMashStep++;
             tempMashTemp =  inputRecipie.mashTemps[curDisplayedMashStep];
             tempMashAmmount = inputRecipie.mashAmmounts[curDisplayedMashStep];
             tempMashMotorOn = inputRecipie.mashMotorStates[curDisplayedMashStep];
             tempMashHours = convertToDisHours(inputRecipie.mashTimes[curDisplayedMashStep]);
             tempMashMins = convertToDisMins(inputRecipie.mashTimes[curDisplayedMashStep]);
             tempMashSecs = convertToDisSecs(inputRecipie.mashTimes[curDisplayedMashStep]);
             tempMoreMashSteps = true;
             return;
          }else
          {
             //newMashStep
             tempMoreMashSteps = false;
             curDisplayedMashStep++;
             tempMoreMashSteps = 0;
             inputRecipie.numberOfMashSteps++;
         
             tempMashTemp = 0;
             tempMashAmmount = 0;
             tempMashMotorOn = false;
             tempMashHours = 0;
             tempMashMins = 0;
             tempMashSecs = 0; 
          }
       }else
       {
         //go to spargeQuestion screen
         tempMoreMashSteps = false;
         inputRecipie.numberOfMashSteps = curDisplayedMashStep;
         currentScreen = spargeQuestScreen;
         return;
       }
       break;
    }
    case spargeQuestionScreen:
    {       
       if(tempHasSparge)
       {
        //go to Sparge data Screen
        lcd.clear();
        curEdit = 0;
        currentScreen = spargeDataScreen; 
       }else
       {         
        //go to Wort Screen and save last mash step and create zero time sparge
        curDisplayedMashStep++;
        inputRecipie.numberOfMashSteps = curDisplayedMashStep;
        inputRecipie.mashTemps[curDisplayedMashStep] = 0;//turn off heater....
        inputRecipie.mashTemps[curDisplayedMashStep-1] = 0;
        inputRecipie.mashMotorStates[curDisplayedMashStep-1] = tempMashMotorOn;//motor state
        inputRecipie.mashMotorStates[curDisplayedMashStep] = false; //turn off during non sparge
        inputRecipie.mashTimes[curDisplayedMashStep-1] = convertToSeconds(tempMashHours,tempMashMins,tempMashSecs);
        inputRecipie.mashTimes[curDisplayedMashStep] = 0;

        tempMashHours = 0;
        tempMashMins = 0;
        tempMashSecs = 0;
        currentScreen = wortScreen;
        lcd.clear();
        curEdit = 0;
       }
       break; 
    }
    
    case spargeDataInputScreen:
    {
      curDisplayedMashStep++;
      inputRecipie.numberOfMashSteps = curDisplayedMashStep;
      inputRecipie.mashTemps[curDisplayedMashStep-1] = tempMashTemp;//hold sparge temp during last mash time...
      inputRecipie.mashTemps[curDisplayedMashStep] = tempMashTemp;
      inputRecipie.mashAmmounts[curDisplayedMashStep] = tempMashAmmount;
      inputRecipie.mashMotorStates[curDisplayedMashStep] = false;
      inputRecipie.mashTimes[curDisplayedMashStep] = 0;
      //go to Wort Screen
      tempMashHours = 0;
      tempMashMins = 0;
      tempMashSecs = 0;
      currentScreen = wortScreen;
      lcd.clear();
      curEdit = 0;
      break;
    }
    
    case wortInputScreen:
    {
     //save wort
     inputRecipie.wortTemp = tempWortTemp;
     inputRecipie.wortTotalSecs = convertToSeconds(tempMashHours,tempMashMins,tempMashSecs);
     inputRecipie.numOfHopSteps=0;
     for(int i=0; i<3; i++)
     {
        if(convertToSeconds(hopIntHours[i],hopIntMins[i],hopIntSecs[i])>0)
        {
         inputRecipie.hopAdditionIntervals[i] = convertToSeconds(hopIntHours[i],hopIntMins[i],hopIntSecs[i]);
         inputRecipie.numOfHopSteps++;
        } 
     }
     curEdit = 0;
     currentScreen = saveQuestionScreen;
     lcd.clear();
     //
     break; 
    }
    
    case saveStartQuestionScreen:
    {
      if(tempSave)
      {
       //Go Save
       lcd.clear();
       lcd.setCursor(0,0);
       lcd.print("I'M THE SAVE SCREEN LUR"); 
      }else
      {
       //start brew!
       setCurrentRecipie(inputRecipie);
       readyToBrew = true;
      }
      break;
    }
  } 
}

void screenBack()
{
   switch(currentScreen.id) 
  {
   case mashInputScreen:
   {
       if(curDisplayedMashStep>1)
       {
         //go back and fill temp vars
        curDisplayedMashStep--;
        tempMashTemp =  inputRecipie.mashTemps[curDisplayedMashStep];
        tempMashAmmount = inputRecipie.mashAmmounts[curDisplayedMashStep];
        tempMashMotorOn = inputRecipie.mashMotorStates[curDisplayedMashStep];
        tempMashHours = convertToDisHours(inputRecipie.mashTimes[curDisplayedMashStep]);
        tempMashMins = convertToDisMins(inputRecipie.mashTimes[curDisplayedMashStep]);
        tempMashSecs = convertToDisSecs(inputRecipie.mashTimes[curDisplayedMashStep]);
        lcd.clear();
        tempMoreMashSteps = true;
       }else
       {
       //go To strike
        curDisplayedMashStep--;
       tempMashTemp = inputRecipie.mashTemps[curDisplayedMashStep];
       tempMashAmmount = inputRecipie.mashAmmounts[curDisplayedMashStep];
       curEdit = 0;
       lcd.clear();
       currentScreen = strikeScreen;
       }
       break; 
   }
   
   case spargeQuestionScreen:
   {
       //go back to Mash and handle loss of sparge if necessary
        if(inputRecipie.mashTemps[curDisplayedMashStep]==0 && inputRecipie.mashTemps[curDisplayedMashStep-1]==0)
        {
        curDisplayedMashStep--;
        }
        currentScreen = mashScreen;
        tempMashTemp =  inputRecipie.mashTemps[curDisplayedMashStep];
        tempMashAmmount = inputRecipie.mashAmmounts[curDisplayedMashStep];
        tempMashMotorOn = inputRecipie.mashMotorStates[curDisplayedMashStep];
        tempMashHours = convertToDisHours(inputRecipie.mashTimes[curDisplayedMashStep]);
        tempMashMins = convertToDisMins(inputRecipie.mashTimes[curDisplayedMashStep]);
        tempMashSecs = convertToDisSecs(inputRecipie.mashTimes[curDisplayedMashStep]);
        lcd.clear();
        tempMoreMashSteps = false; 
        break;
   }
   
   case spargeDataInputScreen:
   {
     //go Back to Sparge Question Screen
     currentScreen = spargeQuestScreen;
     lcd.clear();
     curEdit = 0;
    break; 
   }
   
   case wortInputScreen:
   {
    if(tempHasSparge)
    {
      //go Back To Sparge info Screen
      tempMashTemp = inputRecipie.mashTemps[curDisplayedMashStep];
      tempMashAmmount = inputRecipie.mashAmmounts[curDisplayedMashStep];
      curDisplayedMashStep--;
      currentScreen = spargeDataScreen;
      lcd.clear();
      curEdit = 0;
    }else
    {
     //go Back to Sparge Question Screen
     currentScreen = spargeQuestScreen;
     lcd.clear();
     curEdit = 0;
    }
    break; 
   }
   
   case saveStartQuestionScreen:
   {
    curEdit = 0;
    currentScreen = wortScreen;
    lcd.clear();
    //go back to Wort screen
    break; 
   }
   
  } 
}

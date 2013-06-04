extern const int buttonRight;     
extern const int buttonLeft;    
extern const int buttonUp;    
extern const int buttonDown;      

// Variables will change:
int buttonStateUp;            
int buttonStateDown;             
int buttonStateLeft;             
int buttonStateRight;      

unsigned int holdCounter;

//costant number of ocun
static int numOfReturn = 1;

int lastButtonStateUP = LOW;   
int lastButtonStateDOWN = LOW;  
int lastButtonStateLEFT = LOW;  
int lastButtonStateRIGHT = LOW;   
int increFlag = LOW;
long lastDebounceTimeUp = 0; 
long lastDebounceTimeDown = 0; 
long lastDebounceTimeLeft = 0;  
long lastDebounceTimeRight = 0; 

long debounceDelay = 10;    // the debounce time; increase if the output flickers

void setupButtons() {
//do less
}

boolean resetScreen()
{
 int readingRight = digitalRead(buttonRight);
 int readingLeft = digitalRead(buttonLeft);
 if(readingRight == 1 && readingLeft == 1)
 {
  return true; 
 }
 return false;
}

boolean emergencyShutdown()
{
  boolean upPressed = false;
  boolean downPressed = false;
  
 int readingUp = digitalRead(buttonUp);
 int readingDown = digitalRead(buttonDown);
 
   if (readingUp != lastButtonStateUP) 
  {
    increFlag = LOW;
    lastDebounceTimeUp = millis();
  } 
  
  if (readingDown != lastButtonStateDOWN) 
  {
    increFlag = LOW;
    lastDebounceTimeDown = millis();
  } 

  if ((millis() - lastDebounceTimeUp) > debounceDelay) {
    buttonStateUp = readingUp;
    if(buttonStateUp == HIGH)
     {
       upPressed = true;
     }
  }
  
  if ((millis() - lastDebounceTimeDown) > debounceDelay) {
    buttonStateDown = readingDown;
    if(buttonStateDown == HIGH)
     {
        downPressed = true;
     }
  }
  
   if(upPressed == true && downPressed == true)
   {
     return true;
   }
   return false;
}

//check each for debounce and be able to return which one....have flag for 2 or more etc....
int Buttonloop() {
  // read the state of the switch into a local variable:
  int readingRight = digitalRead(buttonRight);
  int readingLeft = digitalRead(buttonLeft);
  int readingUp = digitalRead(buttonUp);
  int readingDown = digitalRead(buttonDown);

  // If the switch changed, due to noise or pressing:
  if (readingUp != lastButtonStateUP) 
  {
    increFlag = LOW;
    lastDebounceTimeUp = millis();
  } 
  
  if (readingDown != lastButtonStateDOWN) 
  {
    increFlag = LOW;
    lastDebounceTimeDown = millis();
  } 
  
  if (readingLeft != lastButtonStateLEFT) 
  {
    increFlag = LOW;
    lastDebounceTimeLeft = millis();
  } 
  
  if (readingRight != lastButtonStateRIGHT) 
  {
    increFlag = LOW;
    lastDebounceTimeRight = millis();
  } 
  
  if ((millis() - lastDebounceTimeUp) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:
    buttonStateUp = readingUp;
    if(buttonStateUp == HIGH && increFlag < numOfReturn)
     {
       increFlag ++;
       //Serial.println("Pressed up");
       return 1;
     }
  }
  
    if ((millis() - lastDebounceTimeDown) > debounceDelay) {
    buttonStateDown = readingDown;
    if(buttonStateDown == HIGH && increFlag < numOfReturn)
     {
       increFlag ++;
       //Serial.println("Pressed Down");
       return 2;
     }
  }
  
    if ((millis() - lastDebounceTimeLeft) > debounceDelay) {
    buttonStateLeft = readingLeft;
    if(buttonStateLeft == HIGH && increFlag < numOfReturn)
     {
       increFlag ++;
       //Serial.println("Pressed Left");
       return 3;
    }
  }
  
    if ((millis() - lastDebounceTimeRight) > debounceDelay) {
    buttonStateRight = readingRight;
    if(buttonStateRight == HIGH && increFlag < numOfReturn)
     {
       increFlag ++;
       //Serial.println("Pressed Right");
        return  4;
     }
  }
 
  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonStateUP = readingUp;
  lastButtonStateDOWN = readingDown;
  lastButtonStateLEFT = readingLeft;
  lastButtonStateRIGHT = readingRight;

  return -1;
}

int ButtonloopRepeate()
{
 int curButtValue = Buttonloop();
 if (curButtValue != -1)
 {   
   holdCounter++;
   if(holdCounter == 1)
   {
    //Serial.print("Value Output!");
    return curButtValue; 
   }
   if(holdCounter>150 && (curButtValue == 1 || curButtValue == 2))
   {
    return curButtValue; 
   }
 }else
 {
  holdCounter = 0; 
 }
 
 return -1;
}

void turnOnMultiHold()
{
 numOfReturn = 32766;  
}

void turnOffMultiHold()
{
 numOfReturn = 1;  
}



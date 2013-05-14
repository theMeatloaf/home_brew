extern const int buttonRight;     
extern const int buttonLeft;    
extern const int buttonUp;    
extern const int buttonDown;      

// Variables will change:
int buttonStateUp;            
int buttonStateDown;             
int buttonStateLeft;             
int buttonStateRight;      

const int numOfReturn = 1;

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
    increFlag = 0;
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
       Serial.println("Pressed up");
       return 1;
     }
  }
  
    if ((millis() - lastDebounceTimeDown) > debounceDelay) {
    buttonStateDown = readingDown;
    if(buttonStateDown == HIGH && increFlag < numOfReturn)
     {
       increFlag ++;
       Serial.println("Pressed Down");
       return 2;
     }
  }
  
    if ((millis() - lastDebounceTimeLeft) > debounceDelay) {
    buttonStateLeft = readingLeft;
    if(buttonStateLeft == HIGH && increFlag < numOfReturn)
     {
       increFlag ++;
       Serial.println("Pressed Left");
       return 3;
    }
  }
  
    if ((millis() - lastDebounceTimeRight) > debounceDelay) {
    buttonStateRight = readingRight;
    if(buttonStateRight == HIGH && increFlag < numOfReturn)
     {
       increFlag ++;
       Serial.println("Pressed Right");
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



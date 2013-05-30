extern const int outlet1     = 9;
extern const int outlet2     = 10;
extern const int lcdP1       = 31;
extern const int lcdP2       = 33;
extern const int lcdP3       = 41;
extern const int lcdP4       = 39;
extern const int lcdP5       = 37;
extern const int lcdP6       = 35;
extern const int DS18S20_Pin = 7;
extern const int buttonRight = 52;     
extern const int buttonLeft  = 46;    
extern const int buttonUp    = 50;    
extern const int buttonDown  = 48;  
extern const int motorRelay  = 2;  

extern const int spargeValve  = 4;  
extern const int mashValve  = 3;  
extern const int wortValve  = 5;  

extern const int pumpRelayPin = 12;

void setupPins()
{
  pinMode(buttonRight, INPUT);
  pinMode(buttonLeft, INPUT);
  pinMode(buttonUp, INPUT);
  pinMode(buttonDown, INPUT);
  pinMode(outlet1, OUTPUT);  
  pinMode(outlet2, OUTPUT);     
  pinMode(motorRelay, OUTPUT);
  pinMode(spargeValve, OUTPUT);
  pinMode(wortValve,OUTPUT);
  pinMode(mashValve,OUTPUT);
  pinMode(pumpRelayPin,OUTPUT);
}

void turnHeatersOff()
{
    digitalWrite(outlet1,LOW);
    digitalWrite(outlet2,LOW);
}

void openSpargeValve()
{
  digitalWrite(spargeValve,HIGH);
}

boolean spargeValveIsOpen()
{
 if (digitalRead(spargeValve)) return true;
 return false;
}

void closeSpargeValve()
{
  digitalWrite(spargeValve,LOW);
}

void openMashValve()
{
  digitalWrite(mashValve,HIGH);
}

void closeMashValve()
{
 digitalWrite(mashValve,LOW); 
}

void openWortValve()
{
 digitalWrite(wortValve,HIGH); 
}

void closeWortValve()
{
 digitalWrite(wortValve,LOW); 
}

void turnOnPump()
{
 digitalWrite(pumpRelayPin,HIGH); 
}

void turnOffPump()
{
 digitalWrite(pumpRelayPin,LOW); 
}


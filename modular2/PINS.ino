#include <Servo.h> 

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
extern const int hopHopper1 = 24;
extern const int hopHopper2 = 19;
extern const int hopHopper3 = 26;
extern const int pumpRelayPin = 12;

Servo hopServo1;
Servo hopServo2;
Servo hopServo3;

unsigned long hopTimeCounter,hopTimeCounter2,hopTimeCounter3;
unsigned int hopDurationCounter=0;
unsigned int hopDurationCounter2 = 0;
unsigned int hopDurationCounter3 = 0;

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
  
  hopServo1.attach(hopHopper1);
  hopServo2.attach(hopHopper2);
  hopServo2.attach(hopHopper3);
  hopServo1.detach();
  hopServo2.detach();
  hopServo3.detach();

}

void keepServo1Off()
{  
  hopServo1.detach();
}
void keepServo2Off()
{  
  hopServo2.detach();
}
void keepServo3Off()
{  
  hopServo3.detach();
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

void turnOnHopper1()
{
 if(hopDurationCounter == 25)
  {
     if(hopServo1.attached())hopServo1.detach();
     return;
  }
    if(millis() > hopTimeCounter)
    {
      hopDurationCounter++;
     //toggleDirection
     if(hopServo1.read() == 120) hopServo1.write(50);
     else 
     { 
       hopServo1.write(120);
     }
      
      hopTimeCounter = millis()+800;
    }
        hopServo1.attach(hopHopper1);
}

void turnOnHopper2()
{
  if(hopDurationCounter2 == 25)
  {
  if(hopServo2.attached())hopServo2.detach();
  return;
  }
  
  if(millis() > hopTimeCounter2)
  {
    hopDurationCounter2++;
   //toggleDirection
   if(hopServo2.read() == 120) hopServo2.write(50);
   else  
   {
     hopServo2.write(120);
   }
   hopTimeCounter2 = millis()+800;
  }
    hopServo2.attach(hopHopper2);
}

void turnOnHopper3()
{
  if(hopDurationCounter3 == 25)
  {
   if(hopServo3.attached())hopServo3.detach();
   return;
  }
  
  if(millis() > hopTimeCounter3)
  {
    hopDurationCounter3++;
   //toggleDirection
   if(hopServo3.read() == 120) hopServo3.write(50);
   else  
   {
     hopServo3.write(120);
   }
    hopTimeCounter3 = millis()+800;
  }
    hopServo3.attach(hopHopper3);
}

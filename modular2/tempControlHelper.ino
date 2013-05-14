#include <PID_v1.h>

double holdTemp, Input, Output;
const int numReadings = 30;
static boolean IS_ON;
double readings[numReadings]; // the readings from the analog input
int index = 0; // the index of the current reading
double total = 0; // the running total
double input = 0; // the average

//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &holdTemp,200,50,0, DIRECT);

//size for Duty Cycle
int WindowSize = 2000;
unsigned long windowStartTime;

void startTempControl()
{
 windowStartTime = millis();

  setupTempSensor();

 //wait ot be sure getting accurate reading...
 delay(1000);

 //tell the PID to range between 0 and the full window size
 myPID.SetOutputLimits(0, WindowSize);

 //turn the PID on
 myPID.SetMode(AUTOMATIC);

}


void PIDLoop(double temp,int pin,int vessel) {
  holdTemp = temp;
  
   float fTemp = getTempF(getTempNew(vessel));   

    total= total - readings[index]; 
    float temperature = fTemp;
    readings[index] = temperature;
    total= total + readings[index]; 
    
    index = index + 1; 
    
    // if we're at the end of the array...
    if (index >= numReadings) 
    // ...wrap around to the beginning:
    index = 0;
    
    // calculate the average:
    Input = total / numReadings; 

   myPID.Compute();
  
  unsigned long now = millis();
  if(now - windowStartTime>WindowSize)
  { 
    //time to shift the Relay Window
    windowStartTime += WindowSize;
  }
  if(Output > now - windowStartTime)
 { 
   IS_ON = true;
   digitalWrite(pin,HIGH);
 }
  else 
  {
    IS_ON = false;
    digitalWrite(pin,LOW);
  }
}

double getHoldTemp()
{
   return holdTemp;
}

boolean isHeatOn()
{
 return IS_ON; 
}

void tempControllerPrint()
{
    
  //Debug print
  Serial.print(" Goal Temp:");
  Serial.print(holdTemp);
  Serial.print(" Current Temp:");
  Serial.print(getTempF(getTemp()));
}

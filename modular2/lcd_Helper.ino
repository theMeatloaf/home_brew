#include <LiquidCrystal.h>
extern const int lcdP1;
extern const int lcdP2;
extern const int lcdP3;
extern const int lcdP4;
extern const int lcdP5;
extern const int lcdP6;

extern const int spargeVesselTemp, mashVesselTemp,wortVesselTemp, mash, wort, strike;

//TODO: ONLY MAKE TIME DISPLAY WHEN ITS ON THAT STEP YO

LiquidCrystal lcd(lcdP1, lcdP2, lcdP3, lcdP4, lcdP5, lcdP6);
 
 void setupLCD()
 {
  lcd.begin(20, 4);
 }
 
 void displayBootScreen()
 {
   for(int i=9; i<20; i++)
   {
   lcd.setCursor(i, 0);
   lcd.print("*");
   lcd.setCursor(i, 3);
   lcd.print("*");
   lcd.setCursor(19-i, 0);
   lcd.print("*");
   lcd.setCursor(19-i, 3);
   lcd.print("*");
   delay(50);
   }
   
   lcd.setCursor(5,1);
   lcd.print("Welcome ");
   delay(500);
   lcd.print("To");
   delay(500);
   lcd.setCursor(0,2);
   lcd.print("Automated ");
   delay(200);
   lcd.print("Home Brew");

   delay(1700);
   
   changeScreens();

   lcd.setCursor(2,1);
   lcd.print("Please Press Up");
   lcd.setCursor(3,2);
   lcd.print("to start Brew");
 }
 
 void displayTimeAndTempLCD()
 {
   lcd.setCursor(0, 0);
   lcd.print("Current Temp:");
   //lcd.print(getTempF(getTemp()));
   //
   lcd.setCursor(0,1);
   lcd.print("Goal Temp:");
   lcd.print(getHoldTemp());
   
   lcd.setCursor(0,2);
   lcd.print("time ");
   lcd.print(getDisplayHours());
   lcd.print(":");
   lcd.print(getDisplayMins());
   lcd.print(":");
   if(getDisplaySecs()<10)lcd.print("0");
   lcd.print(getDisplaySecs());
 }


 void displayElapsSecsLCD()
 {
   lcd.setCursor(0,0);
   lcd.print("Total Elapsed Secs");
   
   lcd.setCursor(0,1);
   lcd.print(getElapsed());
   lcd.print(" Seconds");
 }
 
 void displaySpargeLCD()
 {
  lcd.setCursor(6,0);
  lcd.print("*SPARGE*");
  
  lcd.setCursor(0,1);
  lcd.print("elapsed:");
  if(currentBrewStage()==strike)
  {
  if(getDisplayHours()<10)lcd.print("0");
  lcd.print(getDisplayHours());
  lcd.print(":");
  if(getDisplayMins()<10)lcd.print("0");
  lcd.print(getDisplayMins());
  lcd.print(":");
  if(getDisplaySecs()<10)lcd.print("0");
  lcd.print(getDisplaySecs());
  }else
  {
   lcd.print("00:00:00"); 
  }
  
  lcd.setCursor(0,2);
  lcd.print("Cur Temp:");
  lcd.print(getTempF(getTempNew(spargeVesselTemp)));
  
  lcd.setCursor(0,3);
  lcd.print("Goal:");
  lcd.print(getHoldTemp());
  
  lcd.print(" Heat:");
  if(isHeatOn())lcd.print("ON ");
  else lcd.print("OFF");
 }
 
 void displayMashLCD()
 {
  lcd.setCursor(7,0);
  lcd.print("*MASH*");
  
   lcd.setCursor(1,1);
  if(getCurrentMashStep()>0 && currentBrewStage()==mash)
  {
  if(getDisplayHours()<10)lcd.print("0");
  lcd.print(getDisplayHours());
  lcd.print(":");
  if(getDisplayMins()<10)lcd.print("0");
  lcd.print(getDisplayMins());
  lcd.print(":");
  if(getDisplaySecs()<10)lcd.print("0");
  lcd.print(getDisplaySecs());
    
  lcd.print("/");
  
  if(getFinalHours()<10)lcd.print("0");
  lcd.print(getFinalHours());
  lcd.print(":");
  if(getFinalMins()<10)lcd.print("0");
  lcd.print(getFinalMins());
  lcd.print(":");
  if(getFinalSecs()<10)lcd.print("0");  
  lcd.print(getFinalSecs());
  }else
  {
  lcd.print("00:00:00/00:00:00"); 
  }
  
  lcd.setCursor(2,2);
  lcd.print("step ");
  lcd.print(getCurrentMashStep());
  lcd.print(" out of ");
  lcd.print(getNumberOfMashSteps()-1);
  
  lcd.setCursor(0,3);
  lcd.print("Motor:");
  if(motorIsOn())lcd.print("ON ");
  else lcd.print("OFF ");
  
  lcd.print("temp:");
  //TODO: MAKE IT GET THE DIFFERENT TEMP
  lcd.print((int)getTempF(getTempNew(mashVesselTemp)));
 }
 
 void displayWortLCD()
 {
  lcd.setCursor(7,0);
  lcd.print("*WORT*");

  lcd.setCursor(1,1);
  if(currentBrewStage()==wort)
  {
  if(getDisplayHours()<10)lcd.print("0");
  lcd.print(getDisplayHours());
  lcd.print(":");
  if(getDisplayMins()<10)lcd.print("0");
  lcd.print(getDisplayMins());
  lcd.print(":");
  if(getDisplaySecs()<10)lcd.print("0");
  lcd.print(getDisplaySecs());
  
  lcd.print("/");
  
  if(getFinalHours()<10)lcd.print("0");
  lcd.print(getFinalHours());
  lcd.print(":");
  if(getFinalMins()<10)lcd.print("0");
  lcd.print(getFinalMins());
  lcd.print(":");
  if(getFinalSecs()<10)lcd.print("0");  
  lcd.print(getFinalSecs());
  }else
  {
  lcd.print("00:00:00/00:00:00");
  }
  
  lcd.setCursor(0,2);
  lcd.print("Cur:");
  lcd.print(getTempF(getTempNew(wortVesselTemp)));
  
  lcd.print("Set:");
  lcd.print(getHoldTemp());
  
  lcd.setCursor(0,3);
  lcd.print("HOPS:");
  
  if(currentHopsStep()<=totalHopsSteps())
  {
    lcd.print(currentHopsStep());
  }
  else
  {
  lcd.print(totalHopsSteps());
  }
  lcd.print("/");
  lcd.print(totalHopsSteps());
    
  lcd.print(" ");
  if(convertToDisHours(currentIntervalTime())<10)lcd.print("0");
  lcd.print(convertToDisHours(currentIntervalTime()));
  lcd.print(":");
  if(convertToDisMins(currentIntervalTime())<10)lcd.print("0");
  lcd.print(convertToDisMins(currentIntervalTime()));
  lcd.print(":");
  if(convertToDisSecs(currentIntervalTime())<10)lcd.print("0");
  lcd.print(convertToDisSecs(currentIntervalTime()));
 }
 
 void displayOverallLCD()
 {
  lcd.setCursor(2,0);
  lcd.print("*OVERALL STATUS*");
 
  lcd.setCursor(0,1);
  lcd.print("Status:");
  lcd.print("OK");
 
  lcd.setCursor(0,2);
  lcd.print("Step:");
  switch(currentBrewStage())
  {
   case strike:lcd.print("Strike     ");break;
   case wort:lcd.print("Wort         ");break;
   case mash:lcd.print("Mash         "); break;
  }
  
  lcd.setCursor(0,3);
  if(convertToDisHours(getAllElapsed())<10)lcd.print("0");
  lcd.print(convertToDisHours(getAllElapsed()));
  lcd.print(":");
  if(convertToDisMins(getAllElapsed())<10)lcd.print("0");
  lcd.print(convertToDisMins(getAllElapsed()));
  lcd.print(":");
  if(convertToDisSecs(getAllElapsed())<10)lcd.print("0");
  lcd.print(convertToDisSecs(getAllElapsed()));

  //TODO: create estimation time
  lcd.print("/03:00:00");
 }
  
 void changeScreens()
 {
     lcd.clear(); 
 }

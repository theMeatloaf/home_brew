#include <LiquidCrystal.h>
extern const int lcdP1;
extern const int lcdP2;
extern const int lcdP3;
extern const int lcdP4;
extern const int lcdP5;
extern const int lcdP6;

//TODO: ONLY MAKE TIME DISPLAY WHEN ITS ON THAT STEP YO

LiquidCrystal lcd(lcdP1, lcdP2, lcdP3, lcdP4, lcdP5, lcdP6);
 
 void setupLCD()
 {
  lcd.begin(20, 4);
 }
 
 void displayTimeAndTempLCD()
 {
   lcd.setCursor(0, 0);
   lcd.print("Current Temp:");
   lcd.print(getTempF(getTemp()));
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
  if(getDisplayHours()<10)lcd.print("0");
  lcd.print(getDisplayHours());
  lcd.print(":");
  if(getDisplayMins()<10)lcd.print("0");
  lcd.print(getDisplayMins());
  lcd.print(":");
  if(getDisplaySecs()<10)lcd.print("0");
  lcd.print(getDisplaySecs());
  
  lcd.setCursor(0,2);
  lcd.print("Cur Temp:");
  lcd.print(getTempF(getTemp()));
  
  lcd.setCursor(0,3);
  lcd.print("Goal:");
  lcd.print(getHoldTemp());
  
  lcd.print(" Heat:");
  if(isHeatOn())lcd.print("ON");
  else lcd.print("OFF");
 }
 
 void displayMashLCD()
 {
  lcd.setCursor(7,0);
  lcd.print("*MASH*");
  
   lcd.setCursor(1,1);
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
  
  lcd.setCursor(2,2);
  lcd.print("step ");
  lcd.print(getCurrentMashStep());
  lcd.print(" out of ");
  lcd.print(getNumberOfMashSteps());
  
  lcd.setCursor(0,3);
  lcd.print("Motor:");
  if(motorIsOn())lcd.print("ON ");
  else lcd.print("OFF ");
  
  lcd.print("temp:");
  //TODO: MAKE IT GET THE DIFFERENT TEMP
  lcd.print((int)getTempF(getTemp()));
 }
 
 void displayWortLCD()
 {
  lcd.setCursor(7,0);
  lcd.print("*WORT*");

  lcd.setCursor(1,1);
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

 
  lcd.setCursor(0,2);
  lcd.print("Cur:");
  lcd.print(getTempF(getTemp()));
  
  lcd.print("Set:");
  lcd.print(getHoldTemp());
  
  lcd.setCursor(0,3);
  lcd.print("HOPS:");
  
  lcd.print(currentHopsStep());
  lcd.print("/");
  lcd.print(totalHopsSteps());
    
  lcd.print(" ");
  lcd.print(convertToDisHours(currentIntervalTime()));
  lcd.print(":");
  lcd.print(convertToDisMins(currentIntervalTime()));
  lcd.print(":");
  lcd.print(convertToDisSecs(currentIntervalTime()));
 }
  
 void changeScreens()
 {
     lcd.clear(); 
 }

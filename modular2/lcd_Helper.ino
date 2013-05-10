#include <LiquidCrystal.h>
extern const int lcdP1;
extern const int lcdP2;
extern const int lcdP3;
extern const int lcdP4;
extern const int lcdP5;
extern const int lcdP6;

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
 
 void changeScreens()
 {
     lcd.clear(); 
 }

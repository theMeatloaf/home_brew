extern const int outlet1     = 9;
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
extern const int spargeVesselTemp = 70;
extern const int mashVesselTemp = 71;
extern const int wortVesselTemp = 72;


void setupPins()
{
  pinMode(buttonRight, INPUT);
  pinMode(buttonLeft, INPUT);
  pinMode(buttonUp, INPUT);
  pinMode(buttonDown, INPUT);
 pinMode(outlet1, OUTPUT);     
 
}

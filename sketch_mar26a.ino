/*
 Establishment of old idea of product conveyor buffer wash procedure.
 Two valves and motor. Nothing fancy, no e-stop or start/stop buttons.
 Those need to implemented before actual use!!!

*/

#include <LiquidCrystal.h>

//IO-pin definitions
int motor= 6;
int valveWater=7;
int valveFoam=8;
int eStop=;
int startButton=;
int stopButton=;

// Variables needed
unsigned int programStep = 0;
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
//unsigned long stepDelay= 70000; // this value is in ms. 70000 = 70 sec.
unsigned long stepDelay= 1000; // 1 sec step delay for testing purposes, fast!
unsigned int button;

// instanciate lcd screen with correct connections.
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);  //define LCD pins (RS, E, D4, D5, D6, D7)


void setup() {
  lcd.begin(20, 4 );                       //initializes the LCD and specifies the dimensions
  pinMode(motor, OUTPUT);
  pinMode(valveWater, OUTPUT);
  pinMode(valveFoam, OUTPUT);

  programStep=1; // We run program after boot by default, change to 0 to disable
}
// ----------------------------
void loop() 
{
  // E-Stop detection
  button=digitalRead(eStop);
  if(button==0) // E-Stop should be high when no emergency at present!!!
  {
    programStep=152;
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" E-stop detected");
    lcd.setCursor(0,1);
    lcd.print(" Reboot to");
    lcd.setCursor(0,2);
    lcd.print(" recover.");

    while(1)
    {
      // If we end up here, we stay here until reboot.
    }
  }
  
  // Start button
  button = digitalRead(startButton);
  if(button==1)
  {
    programStep=1;
  }
  

  // Stop button
  button = digitalRead(stopButton);
  if(button==1)
  {
    programStep=152;
    
  }
   
  // Update current time
  currentMillis = millis();

  // Delay time check, if tripped execute statemachine
  if(currentMillis - previousMillis > (stepDelay))
  {
    //lcd.clear();
    
    // Constant texts to lcd.
    if(programStep>0)
    {
      lcd.setCursor(0,0);
      lcd.print("  Running");
      lcd.setCursor(0,1);
      lcd.print("  Step: ");
      stepDelay= 70000;  //set work cycle delay
  
    }
 
    if(programStep>0)
    {
      // Number of program step to lcd 
      lcd.setCursor(8,1);
      lcd.print(programStep); 

      // State machine implementation
      if(programStep==1) // Start conveyor
      {
        lcd.setCursor(0,2);
        lcd.print("  Run motor");
        digitalWrite(motor, HIGH);
      }
      if(programStep==2) // Water soaking
      {
        lcd.setCursor(0,2);
        lcd.print("  Water soak");
        digitalWrite(valveWater, HIGH);
      }
      if(programStep==12) // Water off, save water and let dirt soften up
      {
        lcd.setCursor(0,2);
        lcd.print("  Dirt softening");
        digitalWrite(valveWater, LOW);
      }
      if(programStep==32) // Foam wash
      {
        lcd.setCursor(0,2);
        lcd.print("  Foam wash     ");
        digitalWrite(valveWater, HIGH);
        digitalWrite(valveFoam, HIGH);
      }
      if(programStep==42) // Foam off
      {
        lcd.setCursor(0,2);
        lcd.print("  Effect time   ");
        digitalWrite(valveWater, LOW);
        digitalWrite(valveFoam, LOW);
      }
      if(programStep==92) // Rinse
      {
        lcd.setCursor(0,2);
        lcd.print("  Rinse         ");
        digitalWrite(valveWater, HIGH);
      }
      if(programStep==105) // Rinse off
      {
        lcd.setCursor(0,2);
        lcd.print("  Run dry       ");
        digitalWrite(valveWater, HIGH);
      }

      if(programStep==152) // Stop after drying run
      {
        lcd.setCursor(0,2);
        lcd.print("  Program stop  ");
        digitalWrite(motor, LOW);
        programStep = 0; // Back to stop state.
      }
      if(programStep>0) // Are we running
      {
        programStep++; // Advance programStep by 1
      }
      previousMillis = currentMillis;

    }
  }
}

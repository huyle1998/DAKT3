#include "server.h"
#include "timer.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define acs712  34
#define zmpt    35
#define relay   32

#define buttonPlus    19
#define buttonMinus   18
#define buttonMode    5

int count = 1000;


bool jump = 1;

void setup()
{  
  Serial.begin(9600);
//  ConfigTimer(1000);
  
  pinMode(acs712, INPUT);
  pinMode(zmpt, INPUT);
  pinMode(relay, OUTPUT);

  pinMode(buttonPlus, INPUT_PULLDOWN);
  pinMode(buttonMinus, INPUT_PULLDOWN);
  pinMode(buttonMode, INPUT_PULLDOWN);  

  lcd.init();
  lcd.backlight();
  lcd.print("Waiting wifi...");  
  digitalWrite(relay, HIGH);
  configServer();
  lcd.clear(); 
  lcd.print("Connected wifi");
  delay(1000);
  digitalWrite(relay, LOW);
  lcd.clear();
}

void loop()
{
  digitalCurrent   = analogRead(acs712);  
  digitalVoltage   = analogRead(zmpt);

//  analogCurrent = ((digitalCurrent/2048)*5000-2500)/66;
  analogCurrent = ((digitalCurrent + 1)*0.0048828125 - 2.5)/0.066;
  analogVoltage = (digitalVoltage/2048)* 5*5.128;
  ComunicateToServer();
  if(digitalRead(buttonMode) == HIGH) changeMode();
  showInformation();
}


void changeMode()
{  
  while (digitalRead(buttonMode) == HIGH);
  lcd.clear();
  lcd.print("Set I max");
  while (jump) // Set I max
  {
    if (digitalRead(buttonPlus) == HIGH)
    {
      while (digitalRead(buttonPlus) == HIGH);
      iMax += 0.1;
      while (digitalRead(buttonPlus) == HIGH);
      lcd.setCursor(0, 1);
      lcd.print("I Max = ");
      lcd.print(iMax, 1);
    }
    if (digitalRead(buttonMinus) == HIGH)
    {
      while (digitalRead(buttonMinus) == HIGH);
      iMax -= 0.1;
      while (digitalRead(buttonMinus) == HIGH);
      lcd.setCursor(0, 1);
      lcd.print("I Max = ");
      lcd.print(iMax, 1);
    }
    if (digitalRead(buttonMode) == HIGH)
      jump = 0;
  }
  while (digitalRead(buttonMode) == HIGH);
  jump = 1;

  lcd.clear();
  lcd.print("Set U max");
  while (jump) // Set U max
  {
    if (digitalRead(buttonPlus) == HIGH)
    {
      while (digitalRead(buttonPlus) == HIGH);
      uMax += 1;
      while (digitalRead(buttonPlus) == HIGH);
      lcd.setCursor(0, 1);
      lcd.print("U Max = ");
      lcd.print(uMax, 0);
    }
    if (digitalRead(buttonMinus) == HIGH)
    {
      while (digitalRead(buttonMinus) == HIGH);
      uMax -= 1;
      while (digitalRead(buttonMinus) == HIGH);
      lcd.setCursor(0, 1);
      lcd.print("U Max = ");
      lcd.print(uMax, 0);
    }
    if (digitalRead(buttonMode) == HIGH)
      jump = 0;
  }
  while (digitalRead(buttonMode) == HIGH);
  jump = 1;
  lcd.clear();
}

void showInformation()
{
  
  if(count == 30000)    // Timer = 0.5s ~ 30000  
  {
    lcd.setCursor(0, 0);
    lcd.print("I = "); lcd.setCursor(4,0);lcd.print(digitalCurrent);
    lcd.setCursor(0, 1);  
    lcd.print("U = "); lcd.setCursor(4,1);lcd.print(analogVoltage); 
    count = 0;
  }
  count++;
//  lcd.clear();
   
}

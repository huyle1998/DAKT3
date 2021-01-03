#include <LiquidCrystal_I2C.h>
#include <math.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define zmpt    35

volatile int interruptCounter;
int totalInterruptCounter;
int count1 = 0, count2 = 0;
int digitalVoltage = 0, Dumax = 0;
long double temp = 0;
double analogVoltage = 0;
 
//hw_timer_t * timer = NULL;
//portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
// 
//void IRAM_ATTR onTimer() {
//  portENTER_CRITICAL_ISR(&timerMux);
//  interruptCounter++;
//  portEXIT_CRITICAL_ISR(&timerMux);
// 
//}

void showInfor();
 
void setup() {
 
  Serial.begin(9600);

  pinMode(zmpt, INPUT);

  lcd.init();
  lcd.backlight();
  lcd.clear();
 
//  timer = timerBegin(0, 80, true);
//  timerAttachInterrupt(timer, &onTimer, true);
//  timerAlarmWrite(timer, 200, true);      // 200 ~ 5kHz
//  timerAlarmEnable(timer);
 
}
 
void loop() {
 
//  if (interruptCounter > 0) {
// 
//    portENTER_CRITICAL(&timerMux);
//    interruptCounter--;
//    portEXIT_CRITICAL(&timerMux);
// 
//    totalInterruptCounter++;
// 
//    // code timer here
//    digitalVolate = analogRead(zmpt);
//    temp += (float)(digitalVolate-3220)/742*250*(digitalVolate-3220)/742*250;
//    count2++;
//    if(count2 == 100)
//    {
//      count2 = 0;
//      analogVolate = (float) sqrt(temp/100);
////      analogVolate = (float) 3*pow(10,-5)*pow(analogVolate,3) - 0.02855*pow(analogVolate,2) + 8.1822*analogVolate - 523.81;
//      temp = 0;
//    }
// 
//  }
//  digitalVolate = analogRead(zmpt);
  Dumax = analogRead(zmpt);
  for (int k=0; k<10000; k++)
  {
    digitalVoltage = analogRead(zmpt);
    if(digitalVoltage > Dumax) Dumax = digitalVoltage;
//    Serial.println("In for");
  }
  analogVoltage = (float)(Dumax-2982)/981*250;
  
//  showInfor();
  count1++;
    if (count1 == 10)
    {
      count1 = 0;
      lcd.setCursor(0, 0);
      lcd.print("U = "); lcd.setCursor(4,0); lcd.print(analogVoltage);
    }
}


void showInfor()
{
  count1++;
  if (count1 == 3000000)
  {
    count1 = 0;
    lcd.setCursor(0, 0);
    lcd.print("U = "); lcd.setCursor(4,0); lcd.print(analogVoltage);
  }
}

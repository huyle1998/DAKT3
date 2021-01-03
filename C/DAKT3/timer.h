// https://techtutorialsx.com/2017/10/07/esp32-arduino-timer-interrupts/
volatile int interruptCounter;
int totalInterruptCounter;
 
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
 
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&timerMux);
 
}
 
void ConfigTimer(int microsecond) { 
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, microsecond, true);
  timerAlarmEnable(timer);
 
}
 
void InteruptTimer_1() {
 
  if (interruptCounter > 0) {
 
    portENTER_CRITICAL(&timerMux);
    interruptCounter--;
    portEXIT_CRITICAL(&timerMux);
 
    totalInterruptCounter++;
 
    // code timer here
    digitalVoltage = analogRead(35);    // 35 = zmpt  
    if(abs(digitalVoltage-oldV) > 200 && oldV>0) digitalVoltage = oldV;
    digitalCurrent = analogRead(34);
    if(abs(digitalCurrent-oldC > 200) && oldC>0)  digitalCurrent = oldC;
    oldV = digitalVoltage; oldC = digitalCurrent;
    temp += (float)(digitalVoltage-2982)/981*250*(digitalVoltage-2982)/981*250;
    temp2 += (float)(digitalCurrent-3140)/956*5*(digitalCurrent-3140)/956*5;
    count2++;
    if(count2 == 100)
    {
      count2 = 0;
      
      analogVoltage = (float) sqrt(temp/100);
      analogVoltage = (float) 1.7556*analogVoltage - 0.046236;
      
      if(analogVoltage < 35) 
      {
        analogVoltage = abs(analogVoltage-32)/100;
      }
      else
      {
        analogVoltage = (float) 0.9162*analogVoltage +16.11;
      }
      temp = 0;

      
      analogCurrent = (float) sqrt(temp2/100);
      analogCurrent = (float) 0.7835*analogCurrent - 0.4598;
      if(analogCurrent < 0.4) 
      {
        analogCurrent = abs(analogCurrent-0.3)/10;
      }
      if(analogCurrent > 3.2)
      {
        analogCurrent = analogCurrent + 0.4;
      }
      temp2 = 0;
//      Serial.println(digitalCurrent);
//      analogCurrent = 3;
//      analogVoltage = (float) 1.7556*analogVoltage - 0.046236;
      
//      Serial.println("In timer 1 == 100");
    }
 
  }
  
}

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
    temp += (float)(digitalVoltage-3220)/742*250*(digitalVoltage-3220)/742*250;
    count2++;
    if(count2 == 100)
    {
      count2 = 0;
      analogVoltage = (float) sqrt(temp/100);
      analogVoltage = (float) 3*pow(10,-5)*pow(analogVoltage,3) - 0.02855*pow(analogVoltage,2) + 8.1822*analogVoltage - 523.81;
      temp = 0;
      Serial.println("In timer 1 == 100");
    }
 
  }
}

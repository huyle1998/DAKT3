//#include <SPI.h>
//#include <SD.h>
//
//File myFile;

#define acs712 34
#define zmpt 35

static int digitalCurrent = 0;
static int digitalVoltage = 0;

int arrayI[10000];
int arrayU[10000];
long int cnt = 0;

void setup() {
  // put your setup code here, to run once:  
  Serial.begin(9600);
  pinMode(acs712, INPUT);
  pinMode(zmpt, INPUT);
  

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalCurrent   = analogRead(acs712);
  digitalVoltage   = analogRead(zmpt);
  arrayI[cnt] =  digitalCurrent;
  arrayU[cnt] =  digitalVoltage;  
  cnt++;

  if (cnt > 10000)
  {
    for(int i=0; i < cnt; i++)
    {
      Serial.println(arrayI[i]);
    }

    delay(20000);
    for(int i=0; i < cnt; i++)
    {
      Serial.println(arrayU[i]);
    }
    cnt = 0;
    delay(20000);
  }
}
  

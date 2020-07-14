/* This code works with ZMPT101B AC voltage sensor module and 128x32 OLED display
 * It permits you to measure any AC voltage up to 250V, BE CAREFUL !!!
 * The functions from Filters library permits you to calculate the True RMS of a signal
 * Refer to www.surtrTech.com or SurtrTech YouTube channel for more details
 */

#include <Filters.h> //Easy library to do the calculations
//#include <SPI.h>     //Libraries for the OLED display
//#include <Wire.h>

#define zmpt    35

float testFrequency = 50;                     // test signal frequency (Hz)
float windowLength = 40.0/testFrequency;     // how long to average the signal, for statistist

int Sensor = 0; //Sensor analog input, here it's A0

float intercept = -0.04; // to be adjusted based on calibration testing
float slope = 0.0405; // to be adjusted based on calibration testing
float current_Volts; // Voltage

unsigned long printPeriod = 1000; //Refresh rate
unsigned long previousMillis = 0;


void setup() {
  Serial.begin( 9600 );    // start the serial port
}

void loop() {
  
  RunningStatistics inputStats;                //Easy life lines, actual calculation of the RMS requires a load of coding
  inputStats. ( windowLength );
   
  while( true ) {   
    Sensor = analogRead(zmpt);  // read the analog in value:
    inputStats.input(Sensor);  // log to Stats function
        
    if((unsigned long)(millis() - previousMillis) >= printPeriod) {
      previousMillis = millis();   // update time every second
            
      Serial.print( "\n" );
      
      current_Volts = intercept + slope * inputStats.sigma(); //Calibartions for offset and amplitude
      current_Volts= current_Volts*(40.3231);                //Further calibrations for the amplitude
      
      Serial.print( "\tVoltage: " );
      Serial.print( current_Volts ); //Calculation and Value display is done the rest is if you're using an OLED display      
     
    }
  }

}

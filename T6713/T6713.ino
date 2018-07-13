

//#include <RTClib.h>
//#include <SD.h>

//Taken from: https://github.com/AmphenolAdvancedSensors/Telaire/tree/T6700_Series
/*
Copyright (c) 2016 Amphenol Advanced Sensors
Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#define CHIPSELECT 10
//RTC_PCF8523 rtc;

#include "Wire.h"
#define T6713_I2C
           //ppm CO2 corrected = ppm CO2 measured * ((Tmeasured*pref) / (pmeasured*Tref))

#ifdef T6713_I2C
#include "T6713.h"
#endif //T6713_I2C


void setup(){
  Serial.begin(115200);  // start serial for output
  Wire.begin();
  
//  if (!SD.begin(CHIPSELECT)) {
//    Serial.println("Card failed, or not present");
//    while(1);
//  }
//
//  if (! rtc.begin()) {
//    Serial.println("Couldn't find RTC");
//    while (1);
//  }
}


void loop() {
  GetCO2PPM();
  //DateTime now = rtc.now();
  Serial.println(CO2ppmValue);
//  Serial.print(',');
//  Serial.print(now.year(), DEC);
//  Serial.print('/');
//  Serial.print(now.month(), DEC);
//  Serial.print('/');
//  Serial.print(now.day(), DEC);
//  Serial.print(',');
//  Serial.print(now.hour(), DEC);
//  Serial.print(':');
//  Serial.print(now.minute(), DEC);
//  Serial.print(':');
//  Serial.print(now.second(), DEC);
//  Serial.println();
   
  delay(15000);
}



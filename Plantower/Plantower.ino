//Taken from here: https://github.com/fu-hsi/PMS

#include <PMS.h>
#include <TimeLib.h>

PMS pms5(Serial1);
PMS pms6(Serial2);
PMS pmsX(Serial3);
PMS::DATA data5;
PMS::DATA data6;
PMS::DATA dataX;

void setup()
{
  Serial.begin(9600);  
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);
  setTime(14,44,0,4,7,2018);  
}

void loop()
{
  while(!(pms5.read(data5))){}
  Serial.print(data5.PM_AE_UG_1_0);
  Serial.print(",");
  Serial.print(data5.PM_AE_UG_2_5);
  Serial.print(",");
  Serial.print(data5.PM_AE_UG_10_0);
  Serial.print(",");
  
  while(!(pms6.read(data6))){}
  Serial.print(data6.PM_AE_UG_1_0);
  Serial.print(",");
  Serial.print(data6.PM_AE_UG_2_5);
  Serial.print(",");
  Serial.print(data6.PM_AE_UG_10_0);
  Serial.print(",");
  
  while(!(pmsX.read(dataX))){}
  Serial.print(dataX.PM_AE_UG_1_0);
  Serial.print(",");
  Serial.print(dataX.PM_AE_UG_2_5);
  Serial.print(",");
  Serial.print(dataX.PM_AE_UG_10_0);
  Serial.print(",");

  Serial.print(hour()); 
  printDigits(minute()); 
  printDigits(second()); 
  Serial.println();

  delay(60000);
}

void printDigits(int digits) {
 // utility function for digital clock display: prints preceding colon and leading 0
 Serial.print(":");
 if (digits < 10)
 Serial.print('0');
 Serial.print(digits);
}

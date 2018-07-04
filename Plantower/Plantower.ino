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
  setTime(12,37,0,4,7,2018);  
}

void loop()
{
  if (pms5.read(data5))
  {
    Serial.print("PMS5003"); Serial.print(" "); Serial.print(hour()); printDigits(minute()); printDigits(second()); Serial.print("\n");
    Serial.print("PM 1.0 (ug/m3): ");
    Serial.println(data5.PM_AE_UG_1_0);

    Serial.print("PM 2.5 (ug/m3): ");
    Serial.println(data5.PM_AE_UG_2_5);

    Serial.print("PM 10.0 (ug/m3): ");
    Serial.println(data5.PM_AE_UG_10_0);

    Serial.println();
    delay(4000);
  }
  if (pms6.read(data6))
  {
    Serial.print("PMS6003"); Serial.print(" "); Serial.print(hour()); printDigits(minute()); printDigits(second()); Serial.print("\n");
    Serial.print("PM 1.0 (ug/m3): ");
    Serial.println(data6.PM_AE_UG_1_0);

    Serial.print("PM 2.5 (ug/m3): ");
    Serial.println(data6.PM_AE_UG_2_5);

    Serial.print("PM 10.0 (ug/m3): ");
    Serial.println(data6.PM_AE_UG_10_0);

    Serial.println();
    delay(4000);
  }
  if (pmsX.read(dataX))
  {
    Serial.print("PMSX003");Serial.print(" "); Serial.print(hour()); printDigits(minute()); printDigits(second()); Serial.print("\n");
    Serial.print("PM 1.0 (ug/m3): ");
    Serial.println(dataX.PM_AE_UG_1_0);

    Serial.print("PM 2.5 (ug/m3): ");
    Serial.println(dataX.PM_AE_UG_2_5);

    Serial.print("PM 10.0 (ug/m3): ");
    Serial.println(dataX.PM_AE_UG_10_0);

    Serial.println();
    delay(4000);
  }
}

void printDigits(int digits) {
 // utility function for digital clock display: prints preceding colon and leading 0
 Serial.print(":");
 if (digits < 10)
 Serial.print('0');
 Serial.print(digits);
}

//https://web.wpi.edu/Pubs/E-project/Available/E-project-042618-120716/unrestricted/MCARVAJAL_SOLAR_POWERED_URBAN_POLLUTION_MAPPING.pdf
#include <TimeLib.h>

char Buffer[29];

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  setTime(12,37,0,4,7,2018);
}

void loop() {
  uint8_t STX,D1,D2,D12,ETX,D14,EOT;
  uint16_t D3,D4,D5,D6,D7,D8,D9,D10,D11;
  uint32_t D13;
  if(Serial1.available()){
     Serial1.readBytesUntil('0x04', Buffer, 29);
     STX = (uint8_t)Buffer[0];
     D1 = (uint8_t)Buffer[1];
     D2 = (uint8_t)Buffer[2];

     D3 = (uint16_t)Buffer[3]<<8;
     D3 |= Buffer[4];

     D4 = (uint16_t)Buffer[5]<<8;
     D4 |= Buffer[6];
     
     D5 = (uint16_t)Buffer[7]<<8;
     D5 |= Buffer[8];
     
     D6 = (uint16_t)Buffer[9]<<8;
     D6 |= Buffer[10];
     
     D7 = (uint16_t)Buffer[11]<<8;
     D7 |= Buffer[12];
     
     D8 = (uint16_t)Buffer[13]<<8;
     D8 |= Buffer[14];
     
     D9 = (uint16_t)Buffer[15]<<8;
     D9 |= Buffer[16];
     
     D10 = (uint16_t)Buffer[17]<<8;
     D10 |= Buffer[18];
     
     D11 = (uint16_t)Buffer[19]<<8;
     D11 |= Buffer[20];
     
     D12 = (uint8_t)Buffer[21];

     D13 = (uint32_t)Buffer[22]<<8;
     D13 |= Buffer[23];
     D13<<8;
     D13 |= Buffer[24];
     D13<<8;
     D13 |= Buffer[25];

     ETX = (uint8_t)Buffer[26];
     D14 = (uint8_t)Buffer[27];
     EOT = (uint8_t)Buffer[28];
  }
  Serial.println("Start");
  Serial.println(STX);
  //Serial.println("Average Mass Concentration (Last 60 Seconds)");
  Serial.println(D5);
  //Serial.println(" ug/m^3");

  //Serial.println();
  //Serial.println("PM1.0 Occupany %");
  Serial.println(D9/100);
  
  //Serial.println();
  //Serial.println("PM2.5 Occupany %");
  Serial.println(D10/100);
  Serial.println(EOT);
  delay(10000);
}

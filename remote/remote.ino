#include <SDS011.h>
#include <RTClib.h>
#include "Adafruit_FONA.h"
#include <SD.h>
#include <SoftwareSerial.h>

//FONA Shield Pin Constants
#define FONA_RX 50
#define FONA_TX 51
#define FONA_RST 4

//Home Station Phone Number
#define HOME_PHONE "+17808500725"
#define PINNUMBER "" //SIM card PIN number
#define MAXTRIES 2

//FONA object initialization
SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

SDS011 my_sds;
RTC_PCF8523 rtc;

//Chipselect pin for data logging shield
#define CHIPSELECT 10

volatile int sent = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  fonaSerial->begin(4800);
  if (! fona.begin(*fonaSerial)) {
    Serial.println(F("Couldn't find FONA"));
    while(1);
  }

  fonaSerial->print("AT+CNMI=2,1\r\n"); //Makes FONA print SMS notification
  fona.enableGPS(true); //Turns on GPS locator

  if (!SD.begin(CHIPSELECT)) {
    Serial.println("Card failed, or not present");
    while(1);
  }

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  my_sds.begin(&Serial1);
}


/**
 * Extracts a received SMS message from the notification string.
 * Deletes the message once extracted.
 */
void extractSMS(char* smsBuffer, char* fonaNotificationBuffer){      
  char* bufPtr = fonaNotificationBuffer;
  int slot = 0;            
  int charCount = 0;

  //Read in the notification string
  do  {
    *bufPtr = fona.read();
    Serial.write(*bufPtr);
    delay(1);
  } while ((*bufPtr++ != '\n') && (fona.available()) && (++charCount < (64-1)));
    
  //Add a terminal NULL to the notification string
  *bufPtr = 0;

  //Read the notification string for a SMS received flag, only continue if it is there
  if (1 == sscanf(fonaNotificationBuffer, "+CMTI: " FONA_PREF_SMS_STORAGE ",%d", &slot)) {
    //pass in appropriate buffers and max lengths
    uint16_t smslen;
    fona.readSMS(slot, smsBuffer, 25, &smslen);
    fona.deleteSMS(slot);//Remember to delete the message once done!
  }
}

void loop() {
    char smsBuffer[25];
    char notifBuffer[64];
    bool AckRec = false;
    int trycount = 0;
    int pollcount = 0;
    float p10, p25;
    float avg_p10 = 0;
    float avg_p25 = 0;
    int error;

    while(pollcount < 6){
      //Get Sensor Data
      error = my_sds.read(&p25, &p10);
      if (!error) {
        avg_p10 += p10;
        avg_p25 += p25;
        pollcount += 1;
        Serial.println(String(p10)+","+String(p25));
      }else{
        Serial.println("Couldn't read from sensor");
      }
      delay(10000); //get a value every 10 seconds
    }

    avg_p10 = avg_p10/6; //average
    avg_p25 = avg_p25/6;
    Serial.println("Averages: "+String(avg_p10)+","+String(avg_p25));
    File datafile = SD.open("datalog.csv", FILE_WRITE);
    if(datafile){
      Serial.println("Saving to SD Card");
      DateTime now = rtc.now();
      datafile.print(String(avg_p10)+","+String(avg_p25)+",");    
      datafile.print(now.year(), DEC);
      datafile.print('/');
      datafile.print(now.month(), DEC);
      datafile.print('/');
      datafile.print(now.day(), DEC);
      datafile.print(',');
      datafile.print(now.hour(), DEC);
      datafile.print(':');
      datafile.print(now.minute(), DEC);
      datafile.print(':');
      datafile.print(now.second(), DEC);
      datafile.println();
      datafile.close();
      Serial.println("Save Success!");
    }else{
      Serial.println("Couldn't open file.");
    }
    
    if(fona.available()){
       memset(smsBuffer, 0, sizeof(smsBuffer));
       extractSMS(smsBuffer, notifBuffer);
       if(smsBuffer[0]=='R'){//Data request from home station
        smsfiledump();
       }
    }
}

void smsfiledump(){
  char smsBuffer[25];
  char notifBuffer[64];
  File datafile = SD.open("datalog.csv", FILE_READ);
  unsigned long curr_time;
  String buffer;
  int skip=0;
  if (datafile){
    while(datafile.available()&&(skip<sent)){
      buffer = datafile.readStringUntil('\n');
    }
    while(datafile.available()){
      buffer = datafile.readStringUntil('\n');
      fona.sendSMS(HOME_PHONE,buffer);
      curr_time = millis();
      while(!(fona.available())&&((millis()-curr_time)<30000)){}
      if(fona.available()){
        memset(smsBuffer, 0, sizeof(smsBuffer));
        extractSMS(smsBuffer, notifBuffer);
        if(smsBuffer[0]!='A'){
          break;
        }else{
          sent+=1;
        }
      }else{
        break;
      }
    }
    datafile.close();
    fona.sendSMS(HOME_PHONE,String('E'));
  }
}


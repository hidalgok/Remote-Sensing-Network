#include "DHT.h"
#include "Adafruit_FONA.h"
#include <PMS.h>
#include <SD.h>
#include <SoftwareSerial.h>

//FONA Shield Pin Constants
#define FONA_RX 2
#define FONA_TX 3
#define FONA_RST 4

//Temp/Hum Sensor Constants
#define DHTPIN 8     // Temp & Humid Sensor
#define DHTTYPE DHT22   // DHT 22 signal pin

//Home Station Phone Number
#define HOME_PHONE "+17809944626"
#define PINNUMBER "" //SIM card PIN number
#define MAXTRIES 2

//FONA object initialization
SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

//Sensor object initialization
DHT dht(DHTPIN, DHTTYPE);


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
    fona.readSMS(slot, smsBuffer, 250, &smslen);
    fona.deleteSMS(slot);//Remember to delete the message once done!
  }
}

void loop() {
    char smsBuffer[250];
    char notifBuffer[64];
    bool AckRec = false;
    int trycount = 0;
    int pollcount = 0;
    int sensorval = 0;

    while(pollcount < 6){
      //Get Sensor Data
      sensorval += 3;
      pollcount += 1;
      delay(30000); //get a value every 30 seconds
    }

    sensorval = sensorval/6; //average
    File datafile = SD.open("datalog.csv", FILE_WRITE);
    if(datafile){
      //save data to data file
      //use RTC to get datetime
      datafile.close();
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
  char smsBuffer[250];
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


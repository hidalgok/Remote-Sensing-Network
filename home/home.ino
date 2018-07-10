/*
 * Created by: Ken Hidalgo
 * Code for the home station.
*/
#include "Adafruit_FONA.h"
//#include <SPI.h>
//#include <SD.h>
#include <SoftwareSerial.h>

//FONA Shield Pin Constants
#define FONA_RX 2
#define FONA_TX 3
#define FONA_RST 4

//SIM Card pin number (if any)
#define PINNUMBER ""

//Chipselect pin for data logging shield
//#define CHIPSELECT 10

/** 
 * The phone numbers of the remote stations are saved here 
 * as a temporary workaround for polling frequency updates.
 * Ideally these would be pulled from the databases and sent
 * to the Arduino through the serial port, but we were experiencing
 * too many issues with that method.
 */ 
const char phonebook[][12] = {"17809944628", "17809944630"};
const int num_stations = 2;

//FONA object initialization
SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

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

//  if (!SD.begin(CHIPSELECT)) {
//    Serial.println("Card failed, or not present");
//    while(1);
//  }
//  
  fonaSerial->print("AT+CNMI=2,1\r\n"); //Makes FONA print SMS notification
  //fileDump();
  clearSlots(); //Clear out any old messages so the station can continue receiving more
}

/**
 * Sends an acknowledgment message to the given phone number
 * For simplicity sake, acknowledgment message is a single character, A
 */
void ackSMS(char* PhoneNum){
    fona.sendSMS(PhoneNum, "A");
}

/**
 * Sends the freqSMS to the given phone number
 */
void changefreqSMS(char* PhoneNum, char* freqSMS){
    fona.sendSMS(PhoneNum, freqSMS);
}

/**
 * Extracts a received SMS message from the notification string.
 * Able to grab the message, sender's phone number, as well as date and time of received message.
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

  //Look through the notification buffer for a SMS received flag, only continue if it is there
  if (1 == sscanf(fonaNotificationBuffer, "+CMTI: " FONA_PREF_SMS_STORAGE ",%d", &slot)) {
    uint16_t smslen;
    fona.readSMS(slot, smsBuffer, 100, &smslen);
    fona.deleteSMS(slot); //Remember to delete the message once done!
  }
}

void loop() {
  unsigned long curr_time = 0;
  char smsBuffer[250];
  char notifBuffer[64];
  bool timeout = false;
  bool done = false;
  Serial.println("Please enter which station you would like data from: ");
  Serial.println("Station 1: 17809944628");
  Serial.println("Station 2: 17809944630");
  while(!(Serial.available())){}
  char ch = Serial.read();
  if (ch == '1'){
    fona.sendSMS("17809944628",'R');
    curr_time = millis();
    while(!(fona.available())&&!(timeout)){
      if((millis()-curr_time)>30000){
        timeout = true;
        Serial.println("Communication Timed Out");
      }
    }
    curr_time = millis();
    while(!(done)&&!(timeout)){
      if(fona.available()){
        memset(smsBuffer, 0, sizeof(smsBuffer));
        extractSMS(smsBuffer, notifBuffer);
        Serial.println(smsBuffer);
        fona.sendSMS("17809944628",'A');
        if(smsBuffer[0]=='E'){
          done = true;
        }
        curr_time = millis();
      }
      if((millis()-curr_time)>30000){
        timeout = true;
        Serial.println("Communication Timed Out");
      }
    }
  }else if(ch == '2'){
    fona.sendSMS("17809944630",'R');
    curr_time = millis();
    while(!(fona.available())&&!(timeout)){
      if((millis()-curr_time)>30000){
        timeout = true;
        Serial.println("Communication Timed Out");
      }
    }
    curr_time = millis();
    while(!(done)&&!(timeout)){
      if(fona.available()){
        memset(smsBuffer, 0, sizeof(smsBuffer));
        extractSMS(smsBuffer, notifBuffer);
        Serial.println(smsBuffer);
        fona.sendSMS("17809944630",'A');
        if(smsBuffer[0]=='E'){
          done = true;
        }
        curr_time = millis();
      }
      if((millis()-curr_time)>30000){
        timeout = true;
        Serial.println("Communication Timed Out");
      }
    }
  }else{
    Serial.println("Sorry, that was not a valid input.");
  }
}   


/**
 * Clears out all SMS slots of FONA
 */
void clearSlots(){
  int i = 0;
  for (i=1; i<=50; i++){
    fona.deleteSMS(i); 
  }
}


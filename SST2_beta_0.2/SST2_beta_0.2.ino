/**************************************************************************
  Copyright (C) 2016  mcbittech
  Wi-Fi Smart Thermostat 2.0 based on Souliss IoT Framework
    -wemos D1 mini
    -DHT22 Temperature & Humidity Sensor

  This example is only supported on ESP8266.
  Originally Developed by mcbittech
***************************************************************************/



#include "SoulissFramework.h"                 // Let the IDE point to the Souliss framework

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "FS.h"                               //SPIFFS
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ArduinoJson.h>
#include <DHT.h>

// Configure the Souliss framework
#include "bconf/MCU_ESP8266.h"                /** Load the code directly on the ESP8266 */
#include "preferences.h"

#if(DYNAMIC_CONNECTION)
  #include "conf/RuntimeGateway.h"            // This node is a Peer and can became a Gateway at runtime
  #include "conf/DynamicAddressing.h"         // Use dynamically assigned addresses
  #include "conf/WEBCONFinterface.h"          // Enable the WebConfig interface
  #include "connection_dynamic.h"
#else
  #include "conf/IPBroadcast.h"
  #include "connection_static.h"
#endif


#include "Souliss.h"
#include "constants.h"
#include "display.h"
#include "language.h"
#include "ntp.h"
#include "read_save.h"
//#include "topics.h"
#include <Arduino.h>
#include <dummy.h>



#include <SoftwareSerial.h>

//VARIABLE DEF
//*************************************************************************
//*************************************************************************

DHT dht(DHTPIN, DHTTYPE);
float temperature = 0;
float humidity = 0;
float temperatureprev = 0;
int humidityprev = 0;
float fValT = 0;
float setpoint_retrieved = 0;
bool brefreshth = 0;

int backLEDvalue = 0;
int backLEDvalueHIGH = BRIGHT_MAX;
int backLEDvalueLOW = BRIGHT_MIN_DEFAULT;
bool FADE = 1;
uint8_t mypayload_len = 0;
uint8_t mypayload[2];
float fTopic_A1_Output;
float fTopic_A2_Output;
float fTopic_A3_Output;
float fTopic_A4_Output;
float fTopic_A5_Output;
float fTopic_A6_Output;

boolean relaystatus=0;
boolean bChildLock = false;
boolean T_or_H_isNan = false;
boolean bFlagBegin = false;
boolean b=0;




void setup()
{
  SERIAL_OUT.begin(115200);
  digitalWrite(14,LOW);
  digitalWrite(12,LOW);
  setupDisplay();

  //SPIFFS
  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  backlightDisplay(1);
  page(0);


  SPIFFS.begin();
  File  sst_spiffs_verifica = SPIFFS.open("/sst_settings.json", "r");
  if (!sst_spiffs_verifica) {
      SERIAL_OUT.println("Non riesco a leggere sst_settings.json! formatto la SPIFFS...");
      SPIFFS.format();
      SERIAL_OUT.println("Spiffs formatted");
      ReadAllSettingsFromPreferences();
      //ReadCronoMatrixSPIFFS();
  }
  else
  {
    ReadAllSettingsFromSPIFFS();
  }

  backlightDisplay(10);

  Initialize();               //Init Souliss Framework

#if(DYNAMIC_CONNECTION)
  DYNAMIC_CONNECTION_Init();
#else
#if(DHCP_OPTION)
  STATIC_CONNECTION_Init_DHCP();
#else
  STATIC_CONNECTION_Init_STATICIP();
#endif
#endif


  //*************************************************************************
  //*************************************************************************
  Set_T52(SLOT_TEMPERATURE);
  Set_T53(SLOT_HUMIDITY);
  Set_T19(SLOT_BRIGHT_DISPLAY);
  Set_T11(SLOT_AWAY);

  backlightDisplay(1);
  
  // Define output pins
  pinMode(RELE, OUTPUT);    // Heater
  dht.begin();




  
  // Init the OTA
  // Set Hostname.
  String hostname(HOSTNAME);
  hostname += String(ESP.getChipId(), HEX);
  #ifdef DEBUG
    SERIAL_OUT.print("set OTA hostname: "); SERIAL_OUT.println(hostname);
  #endif
  ArduinoOTA.setHostname((const char *)hostname.c_str());
  ArduinoOTA.begin();
  yield();
  
  delay(5000);
  //NTP
  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  initNTP();


  page(1);
  backlightDisplay(10);
  bclockOFF();
  getTemp(); 
  sendHour();
  sendDate();
  
}

void loop() {       ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EXECUTEFAST() { 

  UPDATEFAST();

	
  FAST_910ms() {
    sendHour();
	//setpoint_retrieved=getSetpoint();
  }
  
  SHIFT_210ms(0) {

     //TODO
  }

  #if(DYNAMIC_CONNECTION)
    DYNAMIC_CONNECTION_fast();
  #else
    STATIC_CONNECTION_fast();
  #endif

}

EXECUTESLOW() {
  UPDATESLOW();

  SLOW_10s() {
	bclockON();
	getTemp();
	bclockOFF();
  }
  
  SLOW_x10s(2) {
	setpoint_retrieved=getSetpoint();  
	checkRele();
  }

  
  SLOW_50s() {
	sendDate();
	reset_resync();
  }

//  SLOW_15m() {
//      //Risincronizzazione NTP
//      initNTP();
//  }


  #if(DYNAMIC_CONNECTION==1)
    DYNAMIC_CONNECTION_slow();
  #endif

  }
  // Look for a new sketch to update over the air
  ArduinoOTA.handle();
  yield();    
}

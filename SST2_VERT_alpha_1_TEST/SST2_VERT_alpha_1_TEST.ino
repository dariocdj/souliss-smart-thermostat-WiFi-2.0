/**************************************************************************
  Wi-Fi Smart Thermostat 2.0 based on Souliss IoT Framework
    -wemos D1 mini
    -DHT22 Temperature & Humidity Sensor

  This example is only supported on ESP8266.
  Developed by mcbittech
***************************************************************************/

#include "SoulissFramework.h"                 // Let the IDE point to the Souliss framework

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include "FS.h" //SPIFFS
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
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
#include "language.h"
#include "ntp.h"
#include <TimeLib.h>
#include "read_save.h"
#include "topics.h"
#include <Arduino.h>

//VARIABLE DEF
//*************************************************************************
//*************************************************************************

DHT dht(DHTPIN, DHTTYPE);
float temperature = 0;
float humidity = 0;
float fValT = 0;
float setpoint = 0;

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

boolean bChildLock = false;
boolean T_or_H_isNan = false;
boolean bFlagBegin = false;




/*
void EEPROM_Reset() {
  // Erase network configuration parameters from previous use of ZeroConf
  SERIAL_OUT.println("Store_Init");
  Store_Init();
  SERIAL_OUT.println("Store_Clear");
  Store_Clear();
  SERIAL_OUT.println("Store_Commit");
  Store_Commit();
  SERIAL_OUT.println("OK");

  // Print the EEPROM contents, if erase has been succesfull you should see only zeros.
  for (uint16_t i = 0; i < STORE__USABLESIZE; i++)
    SERIAL_OUT.println(Return_8bit(i));

  spiffs_Reset();
  ESP.reset();
}
*/

void subscribeTopics() {
  if (subscribedata(TOPIC1, mypayload, &mypayload_len)) {
    float32((uint16_t*) mypayload,  &fTopic_A1_Output);
    SERIAL_OUT.print("TOPIC1: "); SERIAL_OUT.println(fTopic_A1_Output);
  } else if (subscribedata(TOPIC2, mypayload, &mypayload_len)) {
    float32((uint16_t*) mypayload,  &fTopic_A2_Output);
    SERIAL_OUT.print("TOPIC2: "); SERIAL_OUT.println(fTopic_A2_Output);
  } else if (subscribedata(TOPIC3, mypayload, &mypayload_len)) {
    float32((uint16_t*) mypayload,  &fTopic_A3_Output);
    SERIAL_OUT.print("TOPIC3: "); SERIAL_OUT.println(fTopic_A3_Output);
  } else if (subscribedata(TOPIC4, mypayload, &mypayload_len)) {
    float32((uint16_t*) mypayload,  &fTopic_A4_Output);
    SERIAL_OUT.print("TOPIC4: "); SERIAL_OUT.println(fTopic_A4_Output);
  } else if (subscribedata(TOPIC5, mypayload, &mypayload_len)) {
    float32((uint16_t*) mypayload,  &fTopic_A5_Output);
    SERIAL_OUT.print("TOPIC5: "); SERIAL_OUT.println(fTopic_A5_Output);
  } else if (subscribedata(TOPIC6, mypayload, &mypayload_len)) {
    float32((uint16_t*) mypayload,  &fTopic_A6_Output);
    SERIAL_OUT.print("TOPIC6: "); SERIAL_OUT.println(fTopic_A6_Output);
  }
}


void setSoulissDataChanged() {
  if (data_changed != Souliss_TRIGGED) {
    data_changed = Souliss_TRIGGED;
  }
}


void set_ThermostatModeOn(U8 slot) {
  SERIAL_OUT.println("set_ThermostatModeOn");
  memory_map[MaCaco_OUT_s + slot] |= Souliss_T3n_SystemOn;
  memory_map[MaCaco_OUT_s + slot] &= ~Souliss_T3n_HeatingMode;

  // Trig the next change of the state
  setSoulissDataChanged();
}


void set_ThermostatOff(U8 slot) {
  //memory_map[MaCaco_IN_s + slot] = Souliss_T3n_ShutDown;
  memory_map[MaCaco_OUT_s + SLOT_THERMOSTAT] &= ~ (Souliss_T3n_SystemOn | Souliss_T3n_FanOn1 | Souliss_T3n_FanOn2 | Souliss_T3n_FanOn3 | Souliss_T3n_CoolingOn | Souliss_T3n_HeatingOn);
  setSoulissDataChanged();
}


void set_DisplayMinBright(U8 slot, U8 val) {
  memory_map[MaCaco_OUT_s + slot + 1] = val;
  
  // Trig the next change of the state
  setSoulissDataChanged();
}


boolean getSoulissSystemState() {
  return memory_map[MaCaco_OUT_s + SLOT_THERMOSTAT] & Souliss_T3n_SystemOn;
}


void getTemp() {
  // Read temperature value from DHT sensor and convert from single-precision to half-precision
  fValT = dht.readTemperature();
    SERIAL_OUT.print("ACQ Temperature: "); SERIAL_OUT.println(fValT);
  if (!isnan(fValT)) {
    temperature = fValT; //memorizza temperatura se non è Not A Number
    //Import temperature into T31 Thermostat
    ImportAnalog(SLOT_THERMOSTAT + 1, &temperature);
    ImportAnalog(SLOT_TEMPERATURE, &temperature);
  } else {
    bFlagBegin = true;
  }

  // Read humidity value from DHT sensor and convert from single-precision to half-precision
  fValT = dht.readHumidity();
  SERIAL_OUT.print("acquisizione Humidity: "); SERIAL_OUT.println(fValT);
  if (!isnan(fValT)) {
    humidity = fValT;
    ImportAnalog(SLOT_HUMIDITY, &humidity);
  } else {
    bFlagBegin = true;
  }

  if ( bFlagBegin) {
    //if DHT fail then try to reinit
    dht.begin();
    SERIAL_OUT.println(" dht.begin();");
  }
}


void setSetpoint(float setpoint) {
  //SERIAL_OUT.print("Away: ");SERIAL_OUT.println(memory_map[MaCaco_OUT_s + SLOT_AWAY]);
  if (memory_map[MaCaco_OUT_s + SLOT_AWAY]) {
    //is Away
  } else {
    //is not Away
  }
  Souliss_HalfPrecisionFloating((memory_map + MaCaco_OUT_s + SLOT_THERMOSTAT + 3), &setpoint);
}


void bright(int lum) {
  int val = ((float)lum);
  if (val > 100) val = 100;
  if (val < 0) val = 0;
  //TODO    analogWrite(BACKLED, val);
  Serial1.print("dim=");Serial.print(val);
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff); 

}

void setup()
{

  SERIAL_OUT.begin(115200);

  //SPIFFS
  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  SPIFFS.begin();
  File  sst_spiffs_verifica = SPIFFS.open("/sst_settings.json", "r");
  if (!sst_spiffs_verifica) {
    Serial.println(" ");
    Serial.println("Non riesco a leggere sst_settings.json! formatto la SPIFFS...");
    SPIFFS.format();
    Serial.println("Spiffs formatted");
    ReadAllSettingsFromPreferences();
    //ReadCronoMatrixSPIFFS();
  }
  else
  {
    ReadAllSettingsFromSPIFFS();
    //ReadCronoMatrixSPIFFS();
  }


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

  //set default mode
  Set_Thermostat(SLOT_THERMOSTAT);
  set_ThermostatModeOn(SLOT_THERMOSTAT);
  set_DisplayMinBright(SLOT_BRIGHT_DISPLAY, backLEDvalueLOW);

  // Define output pins
  pinMode(RELE, OUTPUT);    // Heater
  dht.begin();


  //NTP
  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  initNTP();
  delay(1000);
  

  // Init the OTA
  // Set Hostname.
  String hostname(HOSTNAME);
  hostname += String(ESP.getChipId(), HEX);
  SERIAL_OUT.print("set OTA hostname: "); SERIAL_OUT.println(hostname);
  ArduinoOTA.setHostname((const char *)hostname.c_str());
  ArduinoOTA.begin();

}

void loop() {       ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EXECUTEFAST() { 

  UPDATEFAST();

  SHIFT_50ms(0) {
      //set point attuale
      setpoint = Souliss_SinglePrecisionFloating(memory_map + MaCaco_OUT_s + SLOT_THERMOSTAT + 3);
      //Stampa il setpoint solo se il valore dell'encoder è diverso da quello impostato nel T31
      //TODO
  }

  SHIFT_50ms(3) {
      Logic_T19(SLOT_BRIGHT_DISPLAY);
      Logic_T11(SLOT_AWAY);
  }

  SHIFT_110ms(0) {
      //TODO
  }


  SHIFT_110ms(4) {
      //TODO
  }

  SHIFT_210ms(0) {
      //FADE
      if (FADE == 0) {
        //Raggiunge il livello di luminosità minima, che può essere variata anche da SoulissApp
        if ( backLEDvalue != backLEDvalueLOW) {
          if ( backLEDvalue > backLEDvalueLOW) {
            backLEDvalue -= BRIGHT_STEP_FADE_OUT;
          } else {
            backLEDvalue += BRIGHT_STEP_FADE_OUT;
          }
          bright(backLEDvalue);
        }
      } else  if (FADE == 1 && backLEDvalue < backLEDvalueHIGH) {
        backLEDvalue +=  BRIGHT_STEP_FADE_IN;
        bright(backLEDvalue);
      }
  }

  SHIFT_210ms(2) {   // We process the logic and relevant input and output
      Logic_Thermostat(SLOT_THERMOSTAT);
      // Start the heater and the fans
      nDigOut(RELE, Souliss_T3n_HeatingOn, SLOT_THERMOSTAT);    // Heater

      // We are not handling the cooling mode, if enabled by the user, force it back to disable
      if (mOutput(SLOT_THERMOSTAT) & Souliss_T3n_CoolingOn){
        mOutput(SLOT_THERMOSTAT) &= ~Souliss_T3n_CoolingOn;
      }    
  }

  FAST_510ms() {
      // Compare the acquired input with the stored one, send the new value to UI if the difference is greater than the deadband
      Logic_T52(SLOT_TEMPERATURE);
      Logic_T53(SLOT_HUMIDITY);
  }

  FAST_710ms() {
      //HOMESCREEN ////////////////////////////////////////////////////////////////
        //EXIT MENU - Actions
        //write min bright on T19
        //memory_map[MaCaco_OUT_s + SLOT_BRIGHT_DISPLAY + 1] = getDisplayBright();
        SERIAL_OUT.print("Set Display Bright: "); SERIAL_OUT.println(memory_map[MaCaco_OUT_s + SLOT_BRIGHT_DISPLAY + 1]);


          //ON
          SERIAL_OUT.println("Set system ON ");
          set_ThermostatModeOn(SLOT_THERMOSTAT);        // Set System On

          //OFF
          SERIAL_OUT.println("Set system OFF ");
          set_ThermostatOff(SLOT_THERMOSTAT);
        
        memory_map[MaCaco_IN_s + SLOT_THERMOSTAT] = Souliss_T3n_RstCmd;          // Reset
        // Trig the next change of the state
        setSoulissDataChanged();
        SERIAL_OUT.println("Init .....");
      
  }


    SHIFT_210ms(3) {

        //if timeout read value of T19
        backLEDvalueLOW =  memory_map[MaCaco_OUT_s + SLOT_BRIGHT_DISPLAY + 1];
        FADE = 0;
        //HOMESCREEN ////////////////////////////////////////////////////////////////

      
    }

  SHIFT_910ms(1) {
      subscribeTopics();

  }

  #if(DYNAMIC_CONNECTION)
    DYNAMIC_CONNECTION_fast();
  #else
    STATIC_CONNECTION_fast();
  #endif
}

EXECUTESLOW() {
  UPDATESLOW();

  SLOW_50s() {
      getTemp(); 
      //if (getCrono()) {
      //  Serial.println("CRONO: aggiornamento");
      //  setSetpoint(checkNTPcrono(ucg));
      //  setEncoderValue(checkNTPcrono(ucg));
      //  Serial.print("CRONO: setpoint: "); Serial.println(setpoint);
      //}
  }

  SLOW_70s() {
    //TODO
  }


  SLOW_15m() {
      //Sincronizzazione NTP
      yield();
      initNTP();
      yield();
  }


  #if(DYNAMIC_CONNECTION==1)
    DYNAMIC_CONNECTION_slow();
  #endif

  }

  // Look for a new sketch to update over the air
  ArduinoOTA.handle();
  yield();
}



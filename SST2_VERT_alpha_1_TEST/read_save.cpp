#include <Arduino.h>
#include <EEPROM.h>
#include "constants.h"
#include "preferences.h"
#include "read_save.h"
#include "FS.h"
#include <ArduinoJson.h>

boolean bClock;
boolean bSystem;
boolean bCrono;
boolean bProgCrono;
boolean bCronoLearn;
boolean bDayLightSavingTime;
int tZone;
int iDisplayBright;


void save_spiffs_prefs(int json_iDisplayBright, int json_bClock, int json_timeZone, int json_DayLightSavingTime, int json_bCrono, int json_bCronoLearn, int json_bSystem) {
  SPIFFS.begin();
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["Luminosita"] = json_iDisplayBright;
  root["Orologio"] = json_bClock;
  root["Tzone"] = json_timeZone;
  root["DayLightSavingTime"] = json_DayLightSavingTime;
  root["Crono"] = json_bCrono;
  root["CronoLearn"] = json_bCronoLearn;
  root["Dispositivo"] = json_bSystem;
  char buffer[256];
  root.printTo(buffer, sizeof(buffer));

  // open file for writing
  File sst_spiffs = SPIFFS.open("/sst_settings.json", "w");
  if (!sst_spiffs) {
    #ifdef DEBUGDEV
      SERIAL_OUT.println("sst_settings.json open failed");
    #endif
  }
  //qui salvo il buffer su file
  sst_spiffs.println(buffer);
  #ifdef DEBUGDEV
    SERIAL_OUT.print("Salvo in SPIFFS il buffer con i settings :"); SERIAL_OUT.println(buffer);
  #endif
  delay(1);
  //chiudo il file
  sst_spiffs.close();
}

int read_spiffs_prefs(const char*  valuedaleggere) {
  File  sst_spiffs_inlettura = SPIFFS.open("/sst_settings.json", "r");
  if (!sst_spiffs_inlettura) {
    #ifdef DEBUGDEV
      SERIAL_OUT.println("sst_settings.json open failed");
    #endif
    return 0;
  }
  String risultato = sst_spiffs_inlettura.readStringUntil('\n');
  //SERIAL_OUT.print("Ho letto dal file : ");SERIAL_OUT.println(risultato);
  char json[200];
  risultato.toCharArray(json, 200);
  //SERIAL_OUT.print("Ecco l'array json convertito: ");SERIAL_OUT.println(json);
  StaticJsonBuffer<200> jsonBuffer_inlettura;
  JsonObject& root_inlettura = jsonBuffer_inlettura.parseObject(json);
  if (!root_inlettura.success()) {
    #ifdef DEBUGDEV
      SERIAL_OUT.println("parseObject() failed");
    #endif
    return 0;
  }
  //leggo il valore e lo parso:
  int risultatoparsed = root_inlettura[valuedaleggere];
  #ifdef DEBUGDEV
    SERIAL_OUT.print("Spiffs Json parsed value of "); SERIAL_OUT.print(valuedaleggere); SERIAL_OUT.print(" :");SERIAL_OUT.println(risultatoparsed);
  #endif
  sst_spiffs_inlettura.close();
  return risultatoparsed;
}


void spiffs_Reset() {
  #ifdef DEBUGDEV
    SERIAL_OUT.println("SPIFFS Formatting... ");
  #endif
  if (SPIFFS.format()) {
    #ifdef DEBUGDEV
      SERIAL_OUT.println("OK");
    #endif
  } else {
    #ifdef DEBUGDEV
      SERIAL_OUT.println("Fail");
    #endif
  }
}  

void ReadAllSettingsFromPreferences() {
  #ifdef DEBUGDEV
    SERIAL_OUT.println("Read ALL Preferences value....");
  #endif
  iDisplayBright = BRIGHT_MIN_DEFAULT;
  bClock = CLOCK;
  bCrono = CRONO;
  bCronoLearn = CRONOLEARN;
  bSystem = SYSTEM_START_ON;
  tZone = TIME_ZONE;
  bDayLightSavingTime = DAYLIGHTSAVINGTIME;

  save_spiffs_prefs(iDisplayBright, bClock, tZone, bDayLightSavingTime, bCrono, bCronoLearn, bSystem);
}

void ReadAllSettingsFromSPIFFS() {
  //SPIFFS
  #ifdef DEBUGDEV
    SERIAL_OUT.println("Read All Settings From SPIFFS.... ");
  #endif
  iDisplayBright = read_spiffs_prefs("Luminosita");
  bClock = read_spiffs_prefs("Orologio");
  bCrono = read_spiffs_prefs("Crono");
  bCronoLearn = read_spiffs_prefs("CronoLearn");
  bSystem = read_spiffs_prefs("Dispositivo");
  tZone = read_spiffs_prefs("Tzone");
  bDayLightSavingTime = read_spiffs_prefs("DayLightSavingTime");
}






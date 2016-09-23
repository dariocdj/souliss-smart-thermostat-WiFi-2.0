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
  //Serial.print("Ecco i dati in json: ");
  //root.printTo(Serial);
  char buffer[256];
  root.printTo(buffer, sizeof(buffer));
  //Serial.println();

  // open file for writing
  File sst_spiffs = SPIFFS.open("/sst_settings.json", "w");
  if (!sst_spiffs) {
    Serial.println("sst_settings.json open failed");
  }
  //qui salvo il buffer su file
  sst_spiffs.println(buffer);
  Serial.print("Salvo in SPIFFS il buffer con i settings :"); Serial.println(buffer);
  delay(1);
  //chiudo il file
  sst_spiffs.close();
}

int read_spiffs_prefs(const char*  valuedaleggere) {
  File  sst_spiffs_inlettura = SPIFFS.open("/sst_settings.json", "r");
  if (!sst_spiffs_inlettura) {
    Serial.println("sst_settings.json open failed");
    return 0;
  }
  String risultato = sst_spiffs_inlettura.readStringUntil('\n');
  //Serial.print("Ho letto dal file : ");Serial.println(risultato);
  char json[200];
  risultato.toCharArray(json, 200);
  //Serial.print("Ecco l'array json convertito: ");Serial.println(json);
  StaticJsonBuffer<200> jsonBuffer_inlettura;
  JsonObject& root_inlettura = jsonBuffer_inlettura.parseObject(json);
  if (!root_inlettura.success()) {
    Serial.println("parseObject() failed");
    return 0;
  }
  //leggo il valore e lo parso:
  int risultatoparsed = root_inlettura[valuedaleggere];
  Serial.print("Spiffs Json parsed value of "); Serial.print(valuedaleggere); Serial.print(" :");
  Serial.println(risultatoparsed);
  sst_spiffs_inlettura.close();
  return risultatoparsed;
}


void spiffs_Reset() {
  Serial.println("Reset EEPROM");

  Serial.println("SPIFFS Formatting... ");

  if (SPIFFS.format()) {
    Serial.println("OK");
  } else {
    Serial.println("Fail");
  }
}  

void ReadAllSettingsFromPreferences() {
  SERIAL_OUT.println("Read ALL Preferences value....");
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
  SERIAL_OUT.println("Read All Settings From SPIFFS.... ");
  iDisplayBright = read_spiffs_prefs("Luminosita");
  bClock = read_spiffs_prefs("Orologio");
  bCrono = read_spiffs_prefs("Crono");
  bCronoLearn = read_spiffs_prefs("CronoLearn");
  bSystem = read_spiffs_prefs("Dispositivo");
  tZone = read_spiffs_prefs("Tzone");
  bDayLightSavingTime = read_spiffs_prefs("DayLightSavingTime");
}






/**************************************************************************
  Copyright (C) 2016  mcbittech
  Wi-Fi Smart Thermostat 2.0 based on Souliss IoT Framework
    -wemos D1 mini
    -DHT22 Temperature & Humidity Sensor

  This example is only supported on ESP8266.
  Originally Developed by mcbittech
***************************************************************************/


//#include <Arduino.h>
#include <dummy.h>
#include <EEPROM.h>


int read_spiffs_prefs(const char*);
void save_spiffs_prefs(int, int, int, int, int, int, int);
void spiffs_Reset();
void ReadAllSettingsFromPreferences();
void ReadAllSettingsFromSPIFFS();


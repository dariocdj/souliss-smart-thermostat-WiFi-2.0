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

//LANGUAGE
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SPLASH_GW_LINE1 "Modo Gateway" 
#define SPLASH_GW_LINE2 "Preparazione..." 

#define SPLASH_PEER_LINE1 "Modo Peer" 
#define SPLASH_PEER_LINE2 "Connessione... " 

#define SPLASH_NEED_CONF_LINE1 "Eseguire la "
#define SPLASH_NEED_CONF_LINE2 "configurazione "
#define SPLASH_NEED_CONF_LINE3 "via WEB "
#define SPLASH_SSID_LINE1 "Connessione "


#define SETPOINT_TEXT_SETPOINT_SCREEN "SETPOINT"
#define SETPOINT_TEXT "Set: "
#define TEMP_TEXT "T: "
#define HUM_TEXT "H: "
#define SYSTEM_OFF_TEXT "SPENTO  " //due spazi in più per coprire il valore dell'umidità, che rimane sotto. Preferisco non dare il clearscreen.
#define CHILDLOCK_TEXT "LOCKED"



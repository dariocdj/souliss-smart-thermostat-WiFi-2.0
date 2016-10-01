/**************************************************************************
  Copyright (C) 2016  mcbittech
  Wi-Fi Smart Thermostat 2.0 based on Souliss IoT Framework
    -wemos D1 mini
    -DHT22 Temperature & Humidity Sensor

  This example is only supported on ESP8266.
  Originally Developed by mcbittech
***************************************************************************/


#include <Arduino.h>
#include <dummy.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>


#define VERSION "1"
#define HOSTNAME "SST2-Thermostat-OTA-" ///< Hostname. The setup function adds the Chip ID at the end.
//DHT22
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DHTPIN 5
#define DHTTYPE DHT22

//SLOT
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SLOT_THERMOSTAT         0
#define SLOT_TEMPERATURE        5     // This is the memory slot used for the execution of the logic in network_address1
#define SLOT_HUMIDITY           7     // This is the memory slot used for the execution of the logic
#define SLOT_BRIGHT_DISPLAY     9
#define SLOT_AWAY               11


//
#define BRIGHT_MAX 100           //%
#define BRIGHT_MIN_DEFAULT 1     //%
#define BRIGHT_STEP_FADE_IN 5    //%
#define BRIGHT_STEP_FADE_OUT 1


//T31 Commands
#define Souliss_T3n_InSetPoint      0x01
#define Souliss_T3n_OutSetPoint     0x02
#define Souliss_T3n_AsMeasured      0x03
#define Souliss_T3n_Cooling         0x04
#define Souliss_T3n_Heating         0x05
#define Souliss_T3n_FanOff          0x06
#define Souliss_T3n_FanLow          0x07
#define Souliss_T3n_FanMed          0x08
#define Souliss_T3n_FanHigh         0x09
#define Souliss_T3n_FanAuto         0x0A
#define Souliss_T3n_FanManual       0x0B
#define Souliss_T3n_SetTemp         0x0C
#define Souliss_T3n_ShutDown        0x0D

//Souliss Commands
#define Souliss_TRIGGED             0x01


//PIN
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// **** Define here the right pin for your ESP module ****
#define RELE     4


//TIMER DISPLAY
#define TIMER_DISPLAY_SETPOINT 1500

//AWAY
#define TEMP_MINUS 1
#define HYSTERESIS_PLUS 0.2
#define HYSTERESIS_MINUS 0.3

//SERIAL
#define SERIAL_OUT Serial
#define DEBUG                                 //USER DEBUG ON UART0
#define DEBUGDEV                              //DEVELOPMENT DEBUG ON UART0



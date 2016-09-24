/**************************************************************************
  Copyright (C) 2016  mcbittech
  Wi-Fi Smart Thermostat 2.0 based on Souliss IoT Framework
    -wemos D1 mini
    -DHT22 Temperature & Humidity Sensor

  This example is only supported on ESP8266.
  Originally Developed by mcbittech
***************************************************************************/


//DISPLAY
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#include <Arduino.h>
#include <dummy.h>
#include "SoftwareSerial.h"


void calcoloAndamento(float);
void sendTHdisplay(SoftwareSerial,float,float);
void ackdisplay(SoftwareSerial);

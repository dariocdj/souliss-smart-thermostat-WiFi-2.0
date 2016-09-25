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
#include <WiFiUdp.h>
#include <TimeLib.h> 

time_t getNtpTime();
void sendNTPpacket(IPAddress);

String digitalClockDisplay();
String digitalClockDisplay_simple();
String digitalDataDisplay();
String printDigits(int);
int getNTPday();
int getNTPhour();
int getNTPminute();

void initNTP();

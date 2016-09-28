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
#include <Arduino.h>
#include <dummy.h>
#include <SoftwareSerial.h>

void setupDisplay();
void page(int);
void send_T_H_display(float,float);
void ackDisplay();
void backlightDisplay(int);
void sendHour();
void sendDate();
void bclockON();
void bclockOFF();
void get_T_Min_Max(float);
void get_H_Min_Max(float);
void reset_Min_Max();
int arrotonda(float);
int dopovirgola(float);
int getDisplayInt(int);
float getSetpoint();



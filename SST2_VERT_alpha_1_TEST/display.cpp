/**************************************************************************
  Copyright (C) 2016  mcbittech
  Wi-Fi Smart Thermostat 2.0 based on Souliss IoT Framework
    -wemos D1 mini
    -DHT22 Temperature & Humidity Sensor

  This example is only supported on ESP8266.
  Originally Developed by mcbittech
***************************************************************************/


#include <Arduino.h>
#include "display.h"
#include "constants.h"
#include "SoftwareSerial.h"
#include "language.h"
#include "ntp.h"


int arrotonda(const float fI){
  int iIrouded = fI;     
  int result;
  return result = iIrouded;
}

int dopovirgola(const float fI){
  int iIrouded = fI;
  float fIX10 = fI * 10;  
  int iI = fIX10;         
  int result;
  return result = fIX10 - (iIrouded*10);
}

void sendTHdisplay(SoftwareSerial serialDisplay,float t,float h){
  serialDisplay.print("n0.val=");serialDisplay.print(arrotonda(t));
  ackdisplay(serialDisplay);
  serialDisplay.print("n1.val=");serialDisplay.print(dopovirgola(t));
  ackdisplay(serialDisplay);
  serialDisplay.print("n2.val=");serialDisplay.print(arrotonda(h));
  ackdisplay(serialDisplay);
  yield();
    SERIAL_OUT.print("    (arrotonda(t))= ");   SERIAL_OUT.println(arrotonda(t));
    SERIAL_OUT.print("    (dopovirgola(t))= "); SERIAL_OUT.println(dopovirgola(t));
    SERIAL_OUT.print("    (arrotonda(h))= ");   SERIAL_OUT.println(arrotonda(h));
}

void ackdisplay(SoftwareSerial serialDisplay){
  serialDisplay.write(0xff);
  serialDisplay.write(0xff);
  serialDisplay.write(0xff);
}

//CALCOLO ANDAMENTO
///////////////////////////////////////////////////////////////////////////
float deltaT = 0;
float pretemperature = 0;
void calcoloAndamento(float temperature) {
  #ifdef DEBUGDEV
    SERIAL_OUT.println("Refresh DeltaT & Trend Arrow ");
  #endif
  deltaT = temperature - pretemperature;
  #ifdef DEBUGDEV
    SERIAL_OUT.print("DELTAT "); SERIAL_OUT.println(deltaT, DEC);
  #endif
  if (temperature > pretemperature && (deltaT != 0)) {
    #ifdef DEBUGDEV
      SERIAL_OUT.println("Trend Arrow PLUS ");
    #endif
  } else if (deltaT == 0) {
    #ifdef DEBUGDEV
      SERIAL_OUT.println("Trend Arrow NONE ");
    #endif
  }
  else {
    #ifdef DEBUGDEV
      SERIAL_OUT.println("Trend Arrow MINUS ");
    #endif
  }
  pretemperature = temperature;
}
///////////////////////////////////////////////////////////////////////////




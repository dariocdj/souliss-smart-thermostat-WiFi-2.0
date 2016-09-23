#include <Arduino.h>
#include "constants.h"
#include "language.h"
#include "ntp.h"


float arrotonda(const float v)
{
  float vX10 = v * 10;
  int vInt = (int) vX10;
  float diff = abs(vX10 - vInt);
  if (diff < 0.5) {
    return (float) vInt / 10;
  } else {
    return (float)(vInt + 1) / 10;
  }
}

int dopovirgola(const float v)
{
  float vX10 = v * 1;
  int vInt = (int) vX10;
  int result;
  float diff = abs(vX10 - vInt);
  return result = diff * 10;
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
  if (temperature > pretemperature && deltaT || 0) {
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




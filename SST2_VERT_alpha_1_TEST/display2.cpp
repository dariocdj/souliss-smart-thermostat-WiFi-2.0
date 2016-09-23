#include <Arduino.h>
#include "constants.h"
#include "language.h"
#include "ntp.h"

//Var Globali
float oldsetpoint;

float arrotonda2(const float v)
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

int dopovirgola2(const float v)
{
  float vX10 = v * 1;
  int vInt = (int) vX10;
  int result;
  float diff = abs(vX10 - vInt);
  return result = diff * 10;
}

float deltaT = 0;
float pretemperature = 0;
void calcoloAndamento(float temperature) {
  //CALCOLO ANDAMENTO
  ///////////////////////////////////////////////////////////////////////////
  SERIAL_OUT.println("Refresh DeltaT & Trend Arrow ");
  deltaT = temperature - pretemperature;
  Serial.print("DELTAT "); Serial.println(deltaT, DEC);
  if (temperature > pretemperature && deltaT || 0) {
    SERIAL_OUT.println("Trend Arrow PLUS ");
  } else if (deltaT == 0) {

    SERIAL_OUT.println("Trend Arrow NONE ");
  }
  else {

    SERIAL_OUT.println("Trend Arrow MINUS ");
  }
  pretemperature = temperature;
  ///////////////////////////////////////////////////////////////////////////

}





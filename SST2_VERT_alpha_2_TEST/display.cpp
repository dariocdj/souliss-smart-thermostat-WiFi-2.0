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
#include "display.h" 
#include "constants.h"
#include <SoftwareSerial.h>
 
SoftwareSerial serialDisplay(14, 12, false, 256); 

void setupDisplay(){
  serialDisplay.begin(9600);
}



void page(int n){
  serialDisplay.print("page ");serialDisplay.print(n);
  ackDisplay(); 
} 

void send_T_H_display(float t,float h){ 
  serialDisplay.print("n0.val=");serialDisplay.print(arrotonda(t)); 
  ackDisplay(); 
  serialDisplay.print("n1.val=");serialDisplay.print(dopovirgola(t)); 
  ackDisplay(); 
  serialDisplay.print("n2.val=");serialDisplay.print(arrotonda(h));  
  ackDisplay();    
    SERIAL_OUT.print("    (arrotonda(t))= ");SERIAL_OUT.println(arrotonda(t)); 
    SERIAL_OUT.print("    (dopovirgola(t))= ");SERIAL_OUT.println(dopovirgola(t)); 
    SERIAL_OUT.print("    (arrotonda(h))= ");SERIAL_OUT.println(arrotonda(h)); 
} 

void ackDisplay(){
  serialDisplay.write(0xff); 
  serialDisplay.write(0xff); 
  serialDisplay.write(0xff);
}

void backlightDisplay(int l){
  serialDisplay.print("dim=");serialDisplay.print(l);
  ackDisplay();
}
 
int arrotonda(float fI){ 
  int iIrouded = fI;      
  int result; 
  return result = iIrouded; 
} 
 
int dopovirgola(float fI){ 
  int iIrouded = fI; 
  float fIX10 = fI * 10;   
  int iI = fIX10;          
  int result; 
  return result = fIX10 - (iIrouded*10); 
} 
 

/////////////////////////////////////////////////////////////////////////// 
 


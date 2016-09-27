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
#include "constants.h"
#include "display.h" 
#include "ntp.h"
#include <SoftwareSerial.h>
 
SoftwareSerial serialDisplay(14, 12, false, 256); 



void setupDisplay(){
  serialDisplay.begin(9600);
}


void cursore(int cursorvalue){
  serialDisplay.print("h0.val=");serialDisplay.print(cursorvalue);
  ackDisplay(); 
}

void bclockON(){
  serialDisplay.print("vis p0,1");
  ackDisplay();
}

void bclockOFF(){
  serialDisplay.print("vis p0,0");
  ackDisplay();
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
    SERIAL_OUT.print("    T= ");SERIAL_OUT.print(arrotonda(t));SERIAL_OUT.print(".");SERIAL_OUT.print(dopovirgola(t));SERIAL_OUT.print(" deg RH=");SERIAL_OUT.print(arrotonda(h)); SERIAL_OUT.println(" %"); 
} 

void sendHour(){
  serialDisplay.print("n17.val=");serialDisplay.print(getNTPhour()); 
  ackDisplay();
  serialDisplay.print("n18.val=");serialDisplay.print(getNTPminute()); 
  ackDisplay();  
}

void sendDate(){
  serialDisplay.print("n14.val=");serialDisplay.print(getNTPday()); 
  ackDisplay();
  serialDisplay.print("n15.val=");serialDisplay.print(getNTPmonth()); 
  ackDisplay();  
  serialDisplay.print("n16.val=");serialDisplay.print(getNTPyear()-2000); 
  ackDisplay(); 
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
  int iIrounded = fI;      
  int result; 
  return result = iIrounded; 
} 
 
int dopovirgola(float fI){ 
  int iIrouded = fI; 
  float fIX10 = fI * 10;   
  int result; 
  return result = fIX10 - (iIrouded*10); 
} 
 

/////////////////////////////////////////////////////////////////////////// 
 


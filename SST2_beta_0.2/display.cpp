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
#include <Nextion.h>
#include <NextionNumber.h>
#include <SoftwareSerial.h>


 
 
 
float fT_max=0;
float fT_min=99;
int fH_max=0; 
int fH_min=99;
 
SoftwareSerial serialDisplay(14, 12, false, 256); 

//Def var display
Nextion nex(serialDisplay);





void setupDisplay(){
  serialDisplay.begin(9600);
  delay(1000);
  serialDisplay.print("bauds=9600");
  ackDisplay();
  serialDisplay.print("baud=9600");
  ackDisplay();
  
  //nex.init();
}

//LETTURE

void poll_data(){
	nex.poll();
	}
	


NextionNumber Dsetpoint		(nex,1,28,"n3");
NextionNumber Dsetpointdec	(nex,1,29,"n4");
//NextionNumber Dsetpointdec	(nex,1,22,"n0");
float precSetpoint;
float getSetpoint() {
	int val2=0;
	int val1=0;
	float setp;
	
	val1=Dsetpointdec.getValue();
	val2=Dsetpoint.getValue();
	
	SERIAL_OUT.print("Dsetpointdec		= ");SERIAL_OUT.println(val1);
	SERIAL_OUT.print("Dsetpoint		= ");SERIAL_OUT.println(val2);
	setp=(val2*10)+val1;
	return setp/10;
}

//COMANDO

void cursore(int cursorvalue) {
  serialDisplay.print("h0.val=");serialDisplay.print(cursorvalue);
  ackDisplay(); 
}

void bclockON(){
  /*serialDisplay.print("vis p4,1");
  ackDisplay();
  serialDisplay.print("vis p3,1");
  ackDisplay();
  serialDisplay.print("vis p2,1");
  ackDisplay();
  serialDisplay.print("vis p1,1");
  ackDisplay();*/
  serialDisplay.print("vis p0,1");
  ackDisplay();
}

void bclockOFF(){
  serialDisplay.print("vis p0,0");
  ackDisplay();
  serialDisplay.print("vis p1,0");
  ackDisplay();
  serialDisplay.print("vis p2,0");
  ackDisplay();
  serialDisplay.print("vis p3,0");
  ackDisplay();  
  serialDisplay.print("vis p4,0");
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
  #ifdef DEBUG
    SERIAL_OUT.print("    T= ");SERIAL_OUT.print(arrotonda(t));SERIAL_OUT.print(".");SERIAL_OUT.print(dopovirgola(t));SERIAL_OUT.print(" deg RH=");SERIAL_OUT.print(arrotonda(h)); SERIAL_OUT.println(" %"); 
  #endif 
  get_T_Min_Max(t);
  get_H_Min_Max(h);
} 

void get_T_Min_Max(float t){ 
  if (t > fT_max){
	fT_max=t; 
	serialDisplay.print("n19.val=");serialDisplay.print(arrotonda(fT_max));
	ackDisplay();
	serialDisplay.print("n20.val=");serialDisplay.print(dopovirgola(fT_max));
	ackDisplay(); 
  }
  if (t < fT_min){
	fT_min=t;
	serialDisplay.print("n21.val=");serialDisplay.print(arrotonda(fT_min));
	ackDisplay();
	serialDisplay.print("n22.val=");serialDisplay.print(dopovirgola(fT_min));
	ackDisplay();
  } 
  #ifdef DEBUGDEV
    SERIAL_OUT.print("T_MAX= ");SERIAL_OUT.print(fT_max);SERIAL_OUT.print("  T_MIN= ");SERIAL_OUT.println(fT_min);
  #endif 
}

void get_H_Min_Max(float h){
	if (h > fH_max){
		fH_max=h;
	}
	if (h < fH_min){
		fH_min=h;
	}
	#ifdef DEBUGDEV
	SERIAL_OUT.print("H_MAX= ");SERIAL_OUT.print(fH_max);SERIAL_OUT.print("  H_MIN= ");SERIAL_OUT.println(fH_min);
	#endif
}

void reset_Min_Max(){
	fT_max=0;
	fT_min=99;
	fH_max=0;
	fH_min=99;
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
  serialDisplay.write(0XFF); 
  serialDisplay.write(0XFF); 
  serialDisplay.write(0XFF);
  //delay(10);
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
 


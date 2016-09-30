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
 
 
float fT_max=0;
float fT_min=99;
int fH_max=0; 
int fH_min=99;
 
SoftwareSerial serialDisplay(14, 12, false, 256); 



void setupDisplay(){
  serialDisplay.begin(57600);
  //serialDisplay.print("bauds=57600");
}

//LETTURE

int getDisplayInt(int n) {
	uint8_t array[8]= {0};
	serialDisplay.print("get n");serialDisplay.print(n);serialDisplay.print(".val");
	serialDisplay.write(0XFF);
	serialDisplay.write(0XFF);
	serialDisplay.write(0XFF);
	reload_read:
	while (serialDisplay.available()>0) {
		if (serialDisplay.read()==0X71) {
			goto serial_read;
			} else {
			goto reload_read;
		}
	}		
	serial_read:
	serialDisplay.readBytes((char *)array, sizeof(array));
	/*	
	SERIAL_OUT.print("array: ");SERIAL_OUT.print(array[0],HEX);
	SERIAL_OUT.print(" , ");SERIAL_OUT.print(array[1],HEX);
	SERIAL_OUT.print(" , ");SERIAL_OUT.print(array[2],HEX);
	SERIAL_OUT.print(" , ");SERIAL_OUT.print(array[3],HEX);
	SERIAL_OUT.print(" , ");SERIAL_OUT.print(array[4],HEX);
	SERIAL_OUT.print(" , ");SERIAL_OUT.print(array[5],HEX);
	SERIAL_OUT.print(" , ");SERIAL_OUT.print(array[6],HEX);
	SERIAL_OUT.print(" , ");SERIAL_OUT.println(array[7],HEX);
	*/
	if (array[0] == HEAD_NUMBER && array[5] == 0xFF && array[6] == 0xFF && array[7] == 0xFF) {	
		uint32_t numericalData = (array[4] << 24) | (array[3] << 16) | (array[2] << 8) | (array[1]);
		ackDisplay();
		return numericalData;
	}
}


float precSetpoint;
float getSetpoint() {
	int val2=0;
	float val1=0;
	float oldsetpoint;
	val2=getDisplayInt(3)*10;
	val1=val2+getDisplayInt(4);
	if (precSetpoint!=(val1/10)) {
		#ifdef DEBUGDEV
			SERIAL_OUT.print("Setpoint retrieved from display: ");SERIAL_OUT.println(val1/10);
		#endif	
		precSetpoint=val1/10;	
		return val1/10;
	} else {
		return val1/10;
	}		
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
 


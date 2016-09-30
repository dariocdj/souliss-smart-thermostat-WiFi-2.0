/**************************************************************************
  Copyright (C) 2016  mcbittech
  Wi-Fi Smart Thermostat 2.0 based on Souliss IoT Framework
    -wemos D1 mini
    -DHT22 Temperature & Humidity Sensor

  This example is only supported on ESP8266.
  Originally Developed by mcbittech
***************************************************************************/


void checkRele() {
	if (temperature > (setpoint_retrieved+HYSTERESIS_PLUS) && relaystatus==1) {
		SERIAL_OUT.print("	DEACTIVATE RELAY @ ");Serial.print(getNTPhour());Serial.print(":");Serial.print(getNTPminute());
		SERIAL_OUT.print(" Setpoint+H: ");Serial.print(setpoint_retrieved+HYSTERESIS_PLUS);SERIAL_OUT.print(" Temperature: ");Serial.println(temperature);
		digitalWrite(RELE,0);
		relaystatus=0;
	}
	if (temperature < (setpoint_retrieved-HYSTERESIS_MINUS) && relaystatus==0) {
		SERIAL_OUT.print("	ACTIVATE RELAY @ ");Serial.print(getNTPhour());Serial.print(":");Serial.print(getNTPminute());
		SERIAL_OUT.print(" Setpoint-H: ");Serial.print(setpoint_retrieved-HYSTERESIS_MINUS);SERIAL_OUT.print(" Temperature: ");Serial.println(temperature);
		digitalWrite(RELE,1);
		relaystatus=1;
	}
}
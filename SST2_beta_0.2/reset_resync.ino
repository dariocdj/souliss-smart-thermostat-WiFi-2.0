/**************************************************************************
  Copyright (C) 2016  mcbittech
  Wi-Fi Smart Thermostat 2.0 based on Souliss IoT Framework
    -wemos D1 mini
    -DHT22 Temperature & Humidity Sensor

  This example is only supported on ESP8266.
  Originally Developed by mcbittech
***************************************************************************/



void reset_resync() {
	//At 03.00 is time to reset min/max temperature & synching NTP
	int hour=getNTPhour();
	int min=getNTPminute();
	if (hour==3 && min==0) {
		#ifdef DEBUGDEV
		SERIAL_OUT.print("INIT_NTP + RESET MIN-MAX VALUE @ ");Serial.print(getNTPhour());Serial.print(":");Serial.println(getNTPminute());
		#endif
		initNTP();
		reset_Min_Max();
	}
}
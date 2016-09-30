/**************************************************************************
  Copyright (C) 2016  mcbittech
  Wi-Fi Smart Thermostat 2.0 based on Souliss IoT Framework
    -wemos D1 mini
    -DHT22 Temperature & Humidity Sensor

  This example is only supported on ESP8266.
  Originally Developed by mcbittech
***************************************************************************/


void bright(int lum) {
	int va = ((float)lum);
	if (va > 100) va = 100;
	if (va < 0) va = 0;
	#ifdef DEBUGDEV
	SERIAL_OUT.print("display bright= ");SERIAL_OUT.println(va);
	#endif
	backlightDisplay(va);
}
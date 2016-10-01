/**************************************************************************
  Copyright (C) 2016  mcbittech
  Wi-Fi Smart Thermostat 2.0 based on Souliss IoT Framework
    -wemos D1 mini
    -DHT22 Temperature & Humidity Sensor

  This example is only supported on ESP8266.
  Originally Developed by mcbittech
***************************************************************************/

void getTemp() {
	read_another_time:
	// Read temperature value from DHT sensor and convert from single-precision to half-precision
	fValT = dht.readTemperature();
	#ifdef DEBUG
	SERIAL_OUT.print("CHECK Temperature: ");SERIAL_OUT.println(fValT);
	#endif
	if (!isnan(fValT)) {
		temperature = fValT; //memorizza temperatura se non è Not A Number
		//Import temperature into T31 Thermostat
		} else {
		bFlagBegin = true;
	}

	// Read humidity value from DHT sensor and convert from single-precision to half-precision
	fValT = dht.readHumidity();
	#ifdef DEBUG
	SERIAL_OUT.print("CHECK Humidity: ");SERIAL_OUT.println(fValT);
	#endif
	if (!isnan(fValT)) {
		humidity = fValT;
		} else {
		bFlagBegin = true;
	}

	if (bFlagBegin) {
		//if DHT fail then try to reinit
		dht.begin();
		bFlagBegin=false;
		#ifdef DEBUGDEV
		SERIAL_OUT.println(" dht.begin();");
		SERIAL_OUT.println(" read another time");
		#endif
		goto read_another_time;
	}
	int humi = humidity;
	if(temperatureprev!=temperature || humidityprev!=humi){
		#ifdef DEBUG
		Serial.print("    update T&H on: ");Serial.print(getNTPday());Serial.print(".");Serial.print(getNTPmonth());Serial.print(".");Serial.print(getNTPyear());Serial.print(" @ ");
		Serial.print(getNTPhour());Serial.print(":");Serial.println(getNTPminute());
		#endif
		send_T_H_display(temperature,humidity);
	}
	temperatureprev = temperature;
	humidityprev = humi;
}

#include <SoftwareSerial.h>
SoftwareSerial swSerial(14, 12, false, 256);  //Pin 5 e 6 wemos

#include "DHT.h"
#define DHTPIN 5     // Pin DHT22
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);


int brightmin=1;
int brightmax=100;
int brightness = brightmin;    // how bright the LED is
int fadeAmount = brightmin;    // how many points to fade the LED by


int i=0;
float temperatura=-1;
int umidita=0;
int temperatura_arrotondata=0;
int temperaturadec=1;




void setup() {
  // initialize serial:
  Serial.begin(9600);
  swSerial.begin(9600);
swSerial.print("dim=");swSerial.print(1);
  swSerial.write(0xff);
  swSerial.write(0xff);
  swSerial.write(0xff);
  delay(1000);
  swSerial.print("page 1");
  swSerial.write(0xff);
  swSerial.write(0xff);
  swSerial.write(0xff);
swSerial.print("dim=");swSerial.print(100);
  swSerial.write(0xff);
  swSerial.write(0xff);
  swSerial.write(0xff);  
  dht.begin();
}

void loop() {
/*
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);  
*/


swSerial.print("n0.val=");swSerial.print(temperatura_arrotondata);
  swSerial.write(0xff);
  swSerial.write(0xff);
  swSerial.write(0xff);
swSerial.print("n1.val=");swSerial.print(temperaturadec);
  swSerial.write(0xff);
  swSerial.write(0xff);
  swSerial.write(0xff);
swSerial.print("n2.val=");swSerial.print(umidita);
  swSerial.write(0xff);
  swSerial.write(0xff);
  swSerial.write(0xff);    
swSerial.print("dim=");swSerial.print(brightness);
  swSerial.write(0xff);
  swSerial.write(0xff);
  swSerial.write(0xff); 


float temperaturaN = dht.readTemperature();
if (!isnan(temperaturaN)) {
    temperatura = temperaturaN; //memorizza temperatura se non è Not A Number
    //Import temperature into T31 Thermostat
    //ImportAnalog(SLOT_THERMOSTAT + 1, &temperature);
    //ImportAnalog(SLOT_TEMPERATURE, &temperature);
  } else {
    dht.begin();
  }
  
temperaturadec = dopovirgola(temperatura);
temperatura_arrotondata = arrotonda(temperatura);
umidita = dht.readHumidity();


  brightness = brightness + fadeAmount;
  
  if (brightness == brightmin || brightness == brightmax) {
    fadeAmount = -fadeAmount ;
  }

}


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
  int vInt = vX10;
  int result;
  float diff = vX10 - vInt;
  
  return result = diff * 10;
}


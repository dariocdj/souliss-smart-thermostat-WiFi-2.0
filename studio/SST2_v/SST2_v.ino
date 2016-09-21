

#include "DHT.h"

#define DHTPIN 5     // Pin DHT22
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);


int brightmin=1;
int brightmax=100;
int brightness = brightmin;    // how bright the LED is
int fadeAmount = brightmin;    // how many points to fade the LED by


int i=0;
int temperatura=-1;
int temperaturadec=0;
int umidita=0;



void setup() {
  // initialize serial:
  Serial.begin(9600);
Serial.print("dim=");Serial.print(1);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  delay(1000);
  Serial.print("page 1");
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
Serial.print("dim=");Serial.print(100);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);  
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


Serial.print("n0.val=");Serial.print(temperatura);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
Serial.print("n1.val=");Serial.print(temperaturadec);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
Serial.print("n2.val=");Serial.print(umidita);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);    
Serial.print("dim=");Serial.print(brightness);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff); 


temperatura = dht.readTemperature();
temperaturadec = random(0,9);
umidita = dht.readHumidity();
delay(100);

  brightness = brightness + fadeAmount;
  
  if (brightness == brightmin || brightness == brightmax) {
    fadeAmount = -fadeAmount ;
  }

}



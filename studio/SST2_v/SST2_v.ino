

int i=0;
int temperatura=-1;
int temperaturadec=0;
int umidita=0;



void setup() {
  // initialize serial:
  Serial.begin(9600);
  Serial1.begin(9600);
  
  Serial1.print("page 1");
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
  delay(5000);
}

void loop() {

Serial1.print("n0.val=");Serial1.print(temperatura);
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
Serial1.print("n1.val=");Serial1.print(temperaturadec);
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
Serial1.print("n2.val=");Serial1.print(umidita);
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);    


temperatura = random(-1,45);
temperaturadec = random(0,9);
umidita = random(1,99);

delay(50);

}



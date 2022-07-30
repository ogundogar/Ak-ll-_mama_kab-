
#include "config.h"
#include "HX711.h"

HX711 scale(12, 14);

int calibration_factor = 50;
int units;
int ounces;
int gram = 0;
int farkigram;
int kalori;


// set up the 'gram,kalorihesap' feed
AdafruitIO_Feed *Gram = io.feed("gram");
AdafruitIO_Feed *kalorihesap = io.feed("kalorihesap");


void setup() {
  Serial.begin(115200);
  Serial.println("HX711 calibration sketch");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on scale");
  Serial.println("Press + or a to increase calibration factor");
  Serial.println("Press - or z to decrease calibration factor");
//Ağırlık değerini ayarlamak için
  scale.set_scale();
  scale.tare();

  while (! Serial);
  Serial.print("Connecting to Adafruit IO");
  io.connect();
  // wait for a connection
  while (io.status() < AIO_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
 // we are connected
  Serial.println();
  Serial.println(io.statusText());
}
void loop()
{
  //adafruit bağlanmak için
  io.run();
  scale.set_scale(calibration_factor);
  Serial.print("Reading: ");
  units = scale.get_units(), 10;
  if (units < 0)
  {
    units = 0.00;
  }
  ounces = units * 0.035274;
  
  Serial.print(units);
  Serial.print(" grams");
  Serial.print(" calibration_factor: ");
  Serial.print(calibration_factor);
  Serial.println();
  delay(5000);
  if (Serial.available())
  {
    char temp = Serial.read();
    if (temp == '+' || temp == 'a')
      calibration_factor += 1;
    else if (temp == '-' || temp == 'z')
      calibration_factor -= 1;
  }
 if (gram != units)
  {
delay(6000);
    farkigram=0;
    farkigram = gram - units;;
  Serial.print("gram farkı:");
  Serial.println(farkigram);
    kalori = (1,5) * farkigram;
    if(kalori<0)
   {
    kalori=0;
   }
  }

  gram = units;
  Serial.print("sending(gram) -> ");
  Serial.println(gram);
  Serial.print("sending(kaloriHesabı) -> ");
  Serial.println(kalori);
  
  //gram ve kalori değerlerini adafruite kaydetmek için
  
  Gram->save(gram);
  kalorihesap->save(kalori);

  
  kalori=0;
}

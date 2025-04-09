#include "DHT12.h"
#include <Wire.h>
#include <BME280.h>

BME280 bme;
DHT12 DHT;  //  implicit Wire

void setup() {
  Serial.begin(9600);

  bme.begin();
  Wire.begin();
  DHT.begin();

}

void loop() {

  if (millis() - DHT.lastRead() >= 2000)
  {
    int status = DHT.read();
    switch (status)
    {
    case DHT12_OK:
      break;
    case DHT12_ERROR_CHECKSUM:
      Serial.print("Checksum error,\t");
      break;
    case DHT12_ERROR_CONNECT:
      Serial.print("Connect error,\t");
      break;
    case DHT12_MISSING_BYTES:
      Serial.print("Missing bytes,\t");
      break;
    default:
      Serial.print("Unknown error,\t");
      break;
    }
    Serial.print("Humidité: ");
    Serial.print(DHT.getHumidity(), 1);
    Serial.println("%");
    
    Serial.print("Temperature: ");
    Serial.print(bme.readTemp(), 2);
    Serial.println(" degrees C");

    Serial.print("Pressure: ");
    Serial.print(bme.readPressure(), 2);
    Serial.println(" Pa");

    Serial.print("Altitude: ");
    Serial.print(bme.readAltitude(), 2);
    Serial.println("m");

    Serial.println();
  }
  delay(1000);
}

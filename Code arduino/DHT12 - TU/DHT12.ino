#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 21 // GPIO pin for DHT12 data

#define DHTTYPE DHT12

#define RELAYPIN 23 // GPIO pin for relay control

DHT dht(DHTPIN, DHTTYPE);

void setup() {
Serial.begin(9600);
dht.begin();
}

void loop() {
float temperature = dht.readTemperature();
float humidity = dht.readHumidity();
if (isnan(temperature) || isnan(humidity)) {
Serial.println("Failed to read from DHT sensor!");
return;
}
Serial.print("Temperature: ");
Serial.print(temperature);
Serial.println("°C");
Serial.print("Humidity: ");
Serial.print(humidity);
Serial.println("%");

// Trigger fan if temperature exceeds 30°C or humidity exceeds 70

delay(2000); // Wait 2 seconds before next reading
}

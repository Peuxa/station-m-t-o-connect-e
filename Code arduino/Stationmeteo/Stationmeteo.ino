#include "DHT12.h"
#include <Wire.h>
#include <BME280.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>

#define PIN_ANEMO D3
#define OLED_RESET 4
Adafruit_SSD1306 display( OLED_RESET );

BME280 bme;
DHT12 DHT;

#if( SSD1306_LCDHEIGHT != 32 )
#error( "Height incorrect, please fix Adafruit_SSD1306.h!" );
#endif

void setup() {
  Serial.begin(115200); 
  bme.begin();
  Wire.begin();
  DHT.begin();

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN_ANEMO, INPUT_PULLUP);

  // Initialise l’écran OLED I²C à l’adresse 0x3C.
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.display(); // Show initialisation message
}

void loop(){
/*
  *
  *BME 280 + DHT12
  *
  */
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
  /*
  *
  *ANEMOMETRE
  *
  */

  // on détecte les fronts
  static int nbr_fronds=0;
  static bool anemo_vu=LOW;
  if( digitalRead(PIN_ANEMO) != anemo_vu ){ anemo_vu=!anemo_vu;
    nbr_fronds++;
    digitalWrite(LED_BUILTIN, (nbr_fronds&1));  
  }

  static unsigned long next_calcul = millis()+10000;
  if( millis()>=next_calcul ){
      int Total_tours_en_10s = nbr_fronds/4;
      nbr_fronds=0;
      next_calcul += 10000;
      int tours_mn = 6*Total_tours_en_10s;
      float vitesse =  tours_mn * 1;   // f(tours_mn)
      Serial.print("Vitesse = ");
      Serial.print(vitesse);
      Serial.print(" Tr/min");
      Serial.print("\n");
  }
/*
  *
  *GIROUETTE
  *
  */
  int sensorValueg = analogRead(A0);
  Serial.println(sensorValueg);
  Serial.println(getGirouetteAngle(sensorValueg));
  delay(1000); 
/*
  *
  *ECRAN OLED
  *
  */
  afficherDonneesMeteo();
  delay(2000); // Update every 2 seconds (adjust as needed)
}

/*
 * Fonction qui convertit en direction la valeur analogique renvoyée par l'arduino (valeurs fixes)
 * RETURN : direction
 */
String getGirouetteAngle(int value) {
  String direction = "NULL";
  
  if (value > 784 && value < 792) direction = "N";
  if (value > 402 && value < 412) direction = "N-E";
  if (value > 458 && value < 468) direction = "N-O";
  if (value > 78 && value < 85) direction = "O";
  if (value > 88 && value < 98) direction = "E";
  if (value > 60 && value < 70) direction = "S";
  if (value > 180 && value < 190) direction = "S-E";
  if (value > 122 && value < 132) direction = "S-O";

  return direction;
}

void afficherDonneesMeteo( void )
{
  display.clearDisplay();

  // Read sensor data
  float temperature = lireTemperature();
  float pressure = lirePression();
  float humidity = lireHumidite();
  float windSpeed = lireVitesseVent();
  String windDirection = lireDirectionVent();

  // Display temperature
  display.setCursor(0, 0);
  display.print("Temp: ");
  display.print(temperature);
  display.print(" C");

  // Display pressure
  display.setCursor(0, 8);
  display.print("Pression: ");
  display.print(pressure);
  display.print(" hPa");

  // Display humidity
  display.setCursor(0, 16);
  display.print("Humidite: ");
  display.print(humidity);
  display.print(" %");

  // Display wind speed
  display.setCursor(0, 24);
  display.print("Vent: ");
  display.print(windSpeed);
  display.print(" m/s");

  // Display wind direction
  display.setCursor(0, 32); // Adjust Y position if needed for a taller display
  display.print("Dir: ");
  display.print(windDirection);

  display.display();
}

// --- Functions to read sensor data ---
// Replace these with your actual sensor reading functions

float lireTemperature()
{
  // Replace with code to read temperature from your sensor
  // Example for BME280:
  // return bme.readTemperature();
  return 20.5; // Placeholder value
}

float lirePression()
{
  // Replace with code to read pressure from your sensor
  // Example for BME280:
  // return bme.readPressure() / 100.0F; // Convert to hPa
  return 1012.3; // Placeholder value
}

float lireHumidite()
{
  // Replace with code to read humidity from your sensor
  // Example for BME280:
  // return bme.readHumidity();
  return 60.2; // Placeholder value
}

float lireVitesseVent()
{
  // Replace with code to read wind speed from your anemometer
  // This will depend on your specific sensor and how it outputs data (e.g., pulses per second, voltage)
  // You might need to implement pulse counting or analog reading and conversion.
  return 5.8; // Placeholder value
}

String lireDirectionVent()
{
  // Replace with code to read wind direction from your wind vane
  // This will depend on your specific sensor (e.g., analog voltage corresponding to directions)
  // You'll likely need to map the sensor readings to compass directions (N, NE, E, etc.).
  return "NW"; // Placeholder value
}
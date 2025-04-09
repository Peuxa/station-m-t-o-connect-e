#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h> // Required for some sensor libraries (e.g., BME280)
// Include your specific sensor library here
// For example, if using a BME280 sensor:
// #include <Adafruit_BME280.h>

#define OLED_RESET 4
Adafruit_SSD1306 display( OLED_RESET );

// Define your sensor object and pins here
// Example for BME280 (assuming default I2C address 0x76):
// Adafruit_BME280 bme;

// Example for wind speed/direction sensor (adjust pins accordingly):
// const int windSpeedPin = A0; // Analog pin for anemometer
// const int windDirectionPin = A1; // Analog pin for wind vane

#if( SSD1306_LCDHEIGHT != 32 )
#error( "Height incorrect, please fix Adafruit_SSD1306.h!" );
#endif

void setup()
{
  Serial.begin(115200); // For debugging

  // Initialise l’écran OLED I²C à l’adresse 0x3C.
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setContrast(255);
  display.display(); // Show initialisation message

}

void loop()
{
  afficherDonneesMeteo();
  delay(2000); // Update every 2 seconds (adjust as needed)
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
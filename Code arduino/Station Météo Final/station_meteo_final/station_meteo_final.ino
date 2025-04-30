#include <Wire.h>      // Pour I2C (BME280, DHT12)
#include "DHT12.h"     // Pour le capteur DHT12
#include <BME280.h>    // Pour le capteur BME280

#define PIN_ANEMO D3   // Broche pour l'anémomètre

// Objets pour les capteurs
BME280 bme;
DHT12 DHT;

void setup() {
  Serial.begin(9600); // Initialise la communication série UNE SEULE FOIS

  // Initialisation des capteurs
  bme.begin();
  Wire.begin(); // Nécessaire pour DHT12 si I2C
  DHT.begin();

  // Configuration des broches
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN_ANEMO, INPUT_PULLUP);
}

void loop() {
  

  // --- Logique Anémomètre (issue du premier code) ---
  // Détection des fronts
  static int nbr_fronds=0;
  static bool anemo_vu=LOW; // Utilise la variable du code original
  // Lire l'état actuel de la broche
  bool etat_actuel_anemo = digitalRead(PIN_ANEMO);
  // Comparer avec l'état précédent mémorisé dans anemo_vu
  if( etat_actuel_anemo != anemo_vu ){
    anemo_vu = !anemo_vu; // Mettre à jour l'état mémorisé (bascule)
    nbr_fronds++;
    digitalWrite(LED_BUILTIN, (nbr_fronds&1)); // Fait clignoter la LED (utilise le bit de poids faible)
  }

  // Calcul du résultat toutes les 10 secondes
  static unsigned long next_calcul = millis()+10000;
  if( millis()>=next_calcul ){
      // Note: La division par 4 suppose 4 fronts par tour. À vérifier/adapter !
      int Total_tours_en_10s = nbr_fronds/4;
      nbr_fronds=0; // Réinitialise le compteur pour la prochaine période
      next_calcul += 10000; // Prépare le prochain calcul dans 10s
      int tours_mn = 6*Total_tours_en_10s; // Calcul des tours/minute

      // Note: La conversion en vitesse est très simpliste ici (facteur 1). À adapter !
      float vitesse =  tours_mn * 1;  // f(tours_mn) - Gardé comme dans l'original

      // Affichage vitesse
      Serial.print("Vitesse anémo = ");
      Serial.print(vitesse);
      Serial.print(" Tr/min"); // L'unité est Tr/min d'après le calcul, même si variable nommée 'vitesse'
      Serial.print("\n"); // Nouvelle ligne

  }

  // --- Logique Capteurs (issue du second code) ---
  // Lecture et affichage toutes les 2 secondes (basé sur le DHT)
  if (millis() - DHT.lastRead() >= 2000)
  {
    int status = DHT.read(); // Tente la lecture DHT

    // Affichage Humidité (DHT12)
    Serial.print("Humidité: ");
    Serial.print(DHT.getHumidity(), 1); // Garde le format original
    Serial.println("%");

    // Affichage Température (BME280)
    Serial.print("Temperature: ");
    Serial.print(bme.readTemp(), 2); // Garde la fonction et format original
    Serial.println(" degrees C");

    // Affichage Pression (BME280)
    Serial.print("Pressure: ");
    Serial.print(bme.readPressure(), 2); // Garde la fonction et format original
    Serial.println(" Pa");

    // Affichage Altitude (BME280)
    Serial.print("Altitude: ");
    Serial.print(bme.readAltitude(), 2); // Garde la fonction et format original
    Serial.println("m");

    //Girouette
    int sensorValueg = analogRead(A0);
    Serial.print("Direction du vent : ");
    Serial.println(getGirouetteAngle(sensorValueg));

    // Ajout pour séparer les affichages si les timings coïncident
    Serial.println("-----");
  }

}
/*
 * Fonction qui convertit en angle la valeur analogique renvoyée par l'arduino (valeurs fixes)
 * RETURN : angle en degré
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
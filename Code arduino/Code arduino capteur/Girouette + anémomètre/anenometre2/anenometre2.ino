#include <Wire.h>

// Broches analogiques pour l'anémomètre et la girouette
#define PIN_ANEMOMETRE A0
#define PIN_GIROUETTE A1

// Définitions pour les calculs
#define PI 3.1415
#define RAYON 0.07

// Variables pour les compteurs et les mesures
unsigned long previousMillis = 0;
unsigned long previousMillis2 = 0;
unsigned long delaiAnemometre = 3000L;
unsigned long delaiProgramme = 60000L;

float gust = 0;
float wind = 0;
int nbAnemo = 0;
float vitesseVent = 0;
float gir = 0;
int nbGir = 0;

// Fonction pour convertir la lecture de l'anémomètre en m/s
float anemometreToMetersPerSecond(int anemometreValue) {
  // Ceci est un exemple de conversion. Vous devrez l'adapter à votre capteur.
  // Supposons que votre anémomètre donne une plage de valeurs analogiques (0-1023)
  // et que vous connaissez la relation entre ces valeurs et la vitesse du vent.
  // Par exemple, si 0 correspond à 0 m/s et 1023 à une vitesse maximale (ex: 50 m/s)
  // Vous devrez calibrer ces valeurs en fonction de votre capteur spécifique.
  return map(anemometreValue, 0, 1023, 0, 50); // Exemple : conversion en m/s
}

void setup() {
  Serial.begin(9600);
}

void loop() {
  unsigned long currentMillis = millis();

  // Lecture des données de l'anémomètre et de la girouette
  if (currentMillis - previousMillis >= delaiAnemometre) {
    previousMillis = currentMillis;

    // Lecture de la vitesse du vent depuis l'anémomètre (broche A0)
    int anemometreData = analogRead(PIN_ANEMOMETRE);
    vitesseVent = anemometreToMetersPerSecond(anemometreData);
    if (vitesseVent > gust) gust = vitesseVent;
    wind += vitesseVent;
    nbAnemo++;
    Serial.print("Vent: ");
    Serial.print(vitesseVent);
    Serial.println(" m/s");

    // Lecture de la direction du vent depuis la girouette (broche A1)
    int girouetteData = analogRead(PIN_GIROUETTE);
    gir = map(girouetteData, 0, 1023, 0, 360); // Exemple: conversion en degrés
    nbGir++;
    Serial.print("Girouette: ");
    Serial.println(gir);
  }

  // Calculs et envoi des données toutes les minutes
  if (currentMillis - previousMillis2 >= delaiProgramme) {
    previousMillis2 = currentMillis;

    float avgwind = wind / nbAnemo;
    Serial.print("Vent AVG: ");
    Serial.print(avgwind);
    Serial.println(" m/s");
    Serial.print("Gust: ");
    Serial.print(gust);
    Serial.println(" m/s");

    float avggir = gir / nbGir;
    Serial.print("Girouette AVG: ");
    Serial.println(avggir);

    wind = 0;
    gust = 0;
    nbAnemo = 0;
    gir = 0;
    nbGir = 0;
  }
}
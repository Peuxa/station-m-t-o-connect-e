void setup() {
  Serial.begin(9600);
}

void loop() {
  int sensorValueg = analogRead(A0);
  Serial.println(sensorValueg);
  Serial.println(getGirouetteAngle(sensorValueg));
  delay(1000); 
}

/*
 * Fonction qui convertit en angle la valeur analogique renvoyée par l'arduino (valeurs fixes)
 * RETURN : angle en degré
 */
float getGirouetteAngle(int value) {
  String direction = "NULL";
  
  if (value > 784 && value < 792) angle = N;
  if (value > 402 && value < 412) angle = N-E;
  if (value > 458 && value < 468) angle = N-O;
  if (value > 78 && value < 85) angle = O;
  if (value > 88 && value < 98) angle = E;
  if (value > 60 && value < 70) angle = S;
  if (value > 180 && value < 190) angle = S-E;
  if (value > 122 && value < 132) angle = S-O;

  return direction;
}
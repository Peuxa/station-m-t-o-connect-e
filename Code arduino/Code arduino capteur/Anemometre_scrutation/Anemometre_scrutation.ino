#define PIN_ANEMO D3
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN_ANEMO, INPUT_PULLUP);
}

void loop(){
  Serial.begin(9600);

  // on détecte les fronts
  static int nbr_fronds=0;
  static bool anemo_vu=LOW;
  if( digitalRead(PIN_ANEMO) != anemo_vu ){ anemo_vu=!anemo_vu;
    nbr_fronds++;
    digitalWrite(LED_BUILTIN, (nbr_fronds&1));  
  }

  // calcul le résultat toutes les 10 secondes
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

}

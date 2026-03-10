int LEDS[5] = {13, 12, 11, 10, 9};

void setup() {
  for(int i = 0; i < 5; i++){
    pinMode(LEDS[i], OUTPUT);
  }
}

void loop() {

  for(int numero = 0; numero <= 20; numero++){

    for(int bit = 0; bit < 5; bit++){
      
      // Extrae cada bit del número
      int estado = bitRead(numero, bit);
      digitalWrite(LEDS[bit], estado);
      
    }

    delay(1000); // Espera 1 segundo
  }

}
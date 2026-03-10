#define LED_VERDE 13
#define LED_AMARILLO 12
#define LED_ROJO 11
#define BOTON 2

// ----- MÁQUINA DE ESTADOS -----
enum EstadoSistema {
  ESPERANDO,
  EVALUANDO,
  ACCION_A,
  ACCION_B,
  ACCION_C,
  ERROR_PATRON
};

EstadoSistema estadoActual = ESPERANDO;

// ----- VARIABLES DE BOTÓN Y DEBOUNCE -----
int estadoBotonActual = HIGH;
int estadoBotonAnterior = HIGH;
unsigned long tiempoUltimoRebote = 0;
const unsigned long TIEMPO_DEBOUNCE = 50;

// ----- VARIABLES DE PATRONES -----
int pulsaciones = 0;
unsigned long intervalos[3];
unsigned long tiempoUltimoClic = 0;
const unsigned long TIMEOUT = 1500;   

// ----- VARIABLES DE ACCIONES -----
unsigned long tiempoAccion = 0;
int pasoAccion = 0;
bool estadoLed = false;

void setup() {
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARILLO, OUTPUT);
  pinMode(LED_ROJO, OUTPUT);
  pinMode(BOTON, INPUT_PULLUP);
}

void loop() {

  unsigned long tiempoActual = millis();

  // -------- LECTURA CON DEBOUNCE --------
  int lectura = digitalRead(BOTON);

  if (lectura != estadoBotonAnterior) {
    tiempoUltimoRebote = tiempoActual;
  }

  if ((tiempoActual - tiempoUltimoRebote) > TIEMPO_DEBOUNCE) {

    if (lectura != estadoBotonActual) {
      estadoBotonActual = lectura;

      if (estadoBotonActual == LOW && estadoActual == ESPERANDO) {

        if (pulsaciones == 0) {
          pulsaciones = 1;
        } else {
          unsigned long delta = tiempoActual - tiempoUltimoClic;
          if (pulsaciones <= 3) {
            intervalos[pulsaciones - 1] = delta;
          }
          pulsaciones++;
        }

        tiempoUltimoClic = tiempoActual;
      }
    }
  }

  estadoBotonAnterior = lectura;

  // -------- TIMEOUT --------
  if (estadoActual == ESPERANDO &&
      pulsaciones > 0 &&
      (tiempoActual - tiempoUltimoClic > TIMEOUT)) {

    estadoActual = EVALUANDO;
  }

  // -------- MÁQUINA DE ESTADOS --------
  switch (estadoActual) {

    case ESPERANDO:
      break;

    case EVALUANDO:

      // Patrón A → 3 rápidas (<500ms)
      if (pulsaciones == 3 &&
          intervalos[0] < 500 &&
          intervalos[1] < 500) {

        estadoActual = ACCION_A;
        tiempoAccion = tiempoActual;
        digitalWrite(LED_VERDE, HIGH);
      }

      // Patrón B → 2 lentas (600–1200ms)
      else if (pulsaciones == 2 &&
               intervalos[0] >= 600 &&
               intervalos[0] <= 1200) {

        estadoActual = ACCION_B;
        tiempoAccion = tiempoActual;
        digitalWrite(LED_AMARILLO, HIGH);
      }

      // Patrón C → rápido–lento–rápido
      else if (pulsaciones == 4 &&
               intervalos[0] < 500 &&
               intervalos[1] >= 600 && intervalos[1] <= 1200 &&
               intervalos[2] < 500) {

        estadoActual = ACCION_C;
        tiempoAccion = tiempoActual;
        pasoAccion = 0;
        estadoLed = true;
        digitalWrite(LED_ROJO, HIGH);
      }

      else {
        estadoActual = ERROR_PATRON;
        tiempoAccion = tiempoActual;
        pasoAccion = 0;
        estadoLed = true;
        digitalWrite(LED_VERDE, HIGH);
        digitalWrite(LED_AMARILLO, HIGH);
        digitalWrite(LED_ROJO, HIGH);
      }

      pulsaciones = 0;
      break;

    case ACCION_A:
      if (tiempoActual - tiempoAccion >= 1000) {
        digitalWrite(LED_VERDE, LOW);
        estadoActual = ESPERANDO;
      }
      break;

    case ACCION_B:
      if (tiempoActual - tiempoAccion >= 2000) {
        digitalWrite(LED_AMARILLO, LOW);
        estadoActual = ESPERANDO;
      }
      break;

    case ACCION_C:
      if (tiempoActual - tiempoAccion >= 150) {

        tiempoAccion = tiempoActual;
        estadoLed = !estadoLed;
        digitalWrite(LED_ROJO, estadoLed);
        pasoAccion++;

        if (pasoAccion >= 10) {
          digitalWrite(LED_ROJO, LOW);
          estadoActual = ESPERANDO;
        }
      }
      break;

    case ERROR_PATRON:
      if (tiempoActual - tiempoAccion >= 300) {

        tiempoAccion = tiempoActual;
        estadoLed = !estadoLed;

        digitalWrite(LED_VERDE, estadoLed);
        digitalWrite(LED_AMARILLO, estadoLed);
        digitalWrite(LED_ROJO, estadoLed);

        pasoAccion++;

        if (pasoAccion >= 4) {
          digitalWrite(LED_VERDE, LOW);
          digitalWrite(LED_AMARILLO, LOW);
          digitalWrite(LED_ROJO, LOW);
          estadoActual = ESPERANDO;
        }
      }
      break;
  }
}
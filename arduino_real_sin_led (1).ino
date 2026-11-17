// BASURA INTELIGENTE - ARDUINO UNO REAL (sin LEDs)
// Sensor HC-SR04 de 4 pines: Trig -> D8, Echo -> D9
// Servo: Senal -> D10
// Boton manual  (PULL-UP):   un lado a D2, otro a GND -> presionado = LOW
// Boton bloqueo (PULL-DOWN): un lado a 5V, otro a D3, y resistencia 10k de D3 a GND -> presionado = HIGH
#include <Servo.h>

const int TRIG = 8;
const int ECHO = 9;
const int PIN_SERVO = 10;
const int BTN_MANUAL = 2;   // pull-up
const int BTN_BLOQUEO = 3;  // pull-down

const int DISTANCIA_ACTIVACION = 20;        // cm
const int ANGULO_CERRADO = 0;
const int ANGULO_ABIERTO = 90;
const unsigned long TIEMPO_ABIERTA = 3000;  // ms
const unsigned long ANTIREBOTE = 50;        // ms

Servo servo;
bool bloqueada = false;
bool ultimoEstadoBloqueo = LOW;
unsigned long ultimoCambio = 0;

float medirDistancia() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long duracion = pulseIn(ECHO, HIGH, 30000);
  if (duracion == 0) return 999;
  return duracion * 0.0343 / 2;
}

float distanciaFiltrada() {
  float suma = 0;
  for (int i = 0; i < 3; i++) {
    suma += medirDistancia();
    delay(20);
  }
  return suma / 3;
}

// PULL-UP: reposo = HIGH, presionado = LOW
bool botonManualPresionado() {
  return digitalRead(BTN_MANUAL) == LOW;
}

// PULL-DOWN: reposo = LOW, presionado = HIGH (alterna el bloqueo)
void revisarBloqueo() {
  bool lectura = digitalRead(BTN_BLOQUEO);
  if (lectura != ultimoEstadoBloqueo && millis() - ultimoCambio > ANTIREBOTE) {
    ultimoCambio = millis();
    ultimoEstadoBloqueo = lectura;
    if (lectura == HIGH) {
      bloqueada = !bloqueada;
      Serial.println(bloqueada ? "BLOQUEADA" : "DESBLOQUEADA");
    }
  }
}

void abrirTapa() {
  servo.write(ANGULO_ABIERTO);
  unsigned long inicio = millis();
  while (millis() - inicio < TIEMPO_ABIERTA) {
    if (distanciaFiltrada() < DISTANCIA_ACTIVACION) {
      inicio = millis();
    }
  }
  servo.write(ANGULO_CERRADO);
  delay(600);
}

void setup() {
  Serial.begin(9600);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(BTN_MANUAL, INPUT_PULLUP);  // pull-up interna
  pinMode(BTN_BLOQUEO, INPUT);        // pull-down externa de 10k
  servo.attach(PIN_SERVO);
  servo.write(ANGULO_CERRADO);
}

void loop() {
  revisarBloqueo();

  if (bloqueada) {
    servo.write(ANGULO_CERRADO);
    delay(50);
    return;
  }

  float d = distanciaFiltrada();
  Serial.println(d);

  if (d < DISTANCIA_ACTIVACION || botonManualPresionado()) {
    abrirTapa();
  }
  delay(100);
}

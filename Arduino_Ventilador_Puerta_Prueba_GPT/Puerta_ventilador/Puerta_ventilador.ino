#include <DHT.h>

// ----------- DHT ------------
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ----------- Ventilador ------------
int ventilador = 3;
int ledRojo = 4;
int ledVerde = 5;
int ledNaranja = 6;

// ----------- Puerta ------------
int motorA = 7;
int motorB = 8;

// Botones
int btnAbrir = 9;
int btnCerrar = 10;

// Simulación finales de carrera
int fcAbierto = 11;
int fcCerrado = 12;

// Sensor infrarrojo
int infrarrojo = 13;

// ----------- Variables ------------
bool moviendo = false;
unsigned long tiempoParpadeo = 0;
bool estadoLED = false;

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(ventilador, OUTPUT);
  pinMode(ledRojo, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledNaranja, OUTPUT);

  pinMode(motorA, OUTPUT);
  pinMode(motorB, OUTPUT);

  pinMode(btnAbrir, INPUT_PULLUP);
  pinMode(btnCerrar, INPUT_PULLUP);
  pinMode(fcAbierto, INPUT_PULLUP);
  pinMode(fcCerrado, INPUT_PULLUP);
  pinMode(infrarrojo, INPUT_PULLUP);
}

// ---------------- LOOP ----------------
void loop() {

  // ====== SISTEMA VENTILADOR ======
  float temp = dht.readTemperature();

  if (!isnan(temp)) {
    Serial.print("Temp: ");
    Serial.println(temp);

    if (temp > 28) {
      digitalWrite(ventilador, HIGH);
      digitalWrite(ledRojo, HIGH);
      digitalWrite(ledVerde, LOW);
      digitalWrite(ledNaranja, LOW);
    }
    else if (temp < 25) {
      digitalWrite(ventilador, LOW);
      digitalWrite(ledRojo, LOW);
      digitalWrite(ledVerde, HIGH);
      digitalWrite(ledNaranja, LOW);
    }
    else {
      digitalWrite(ventilador, LOW);
      digitalWrite(ledRojo, LOW);
      digitalWrite(ledVerde, LOW);
      digitalWrite(ledNaranja, HIGH);
    }
  }

  // ====== SISTEMA PUERTA ======
  bool abrir = !digitalRead(btnAbrir);
  bool cerrar = !digitalRead(btnCerrar);
  bool abierto = !digitalRead(fcAbierto);
  bool cerrado = !digitalRead(fcCerrado);
  bool obstaculo = !digitalRead(infrarrojo);

  // ---- Abrir ----
  if (abrir && !abierto) {
    digitalWrite(motorA, HIGH);
    digitalWrite(motorB, LOW);
    moviendo = true;
  }

  // ---- Cerrar ----
  else if (cerrar && !cerrado && !obstaculo) {
    digitalWrite(motorA, LOW);
    digitalWrite(motorB, HIGH);
    moviendo = true;
  }

  // ---- Parar ----
  else {
    digitalWrite(motorA, LOW);
    digitalWrite(motorB, LOW);
    moviendo = false;
  }

  // ====== LÓGICA INFRARROJO ======

  if (obstaculo) {

    if (moviendo) {
      // LED rojo intermitente
      if (millis() - tiempoParpadeo > 500) {
        estadoLED = !estadoLED;
        digitalWrite(ledRojo, estadoLED);
        tiempoParpadeo = millis();
      }
    }
    else {
      // LED rojo fijo
      digitalWrite(ledRojo, HIGH);
    }
  }
  else {
    // Sin obstáculo → LED verde
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledRojo, LOW);
  }
}
#include <DHT.h>
#include <LiquidCrystal.h>

// -------- LCD (RS, E, D4, D5, D6, D7) --------
LiquidCrystal lcd(6, 7, 8, 9, 10, 11);

// -------- PINES --------
#define TRIG 9
#define ECHO 10

#define DHTPIN 4
#define DHTTYPE DHT11

#define FAN_LED  6
#define MOTOR    3
#define POT_PIN  A0

DHT dht(DHTPIN, DHTTYPE);

// -------- VARIABLES --------
float distancia;
float nivel;
float temp;
float hum;

float dist_min = 4.0;
float dist_max = 22.0;

String estadoSilo = "ERROR";
bool fanActivo = false;

// -------- SETUP --------
void setup() {
  Serial.begin(115200);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(FAN_LED, OUTPUT);
  pinMode(MOTOR, OUTPUT);

  digitalWrite(FAN_LED, LOW);
  analogWrite(MOTOR, 0);

  pinMode(DHTPIN, INPUT_PULLUP);
  dht.begin();

  // LCD
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Sistema listo");
  delay(2000);
  lcd.clear();
}

// -------- LOOP --------
void loop() {
  leerSilo();
  leerClima();
  controlClima();
  enviarDatos();

  delay(500);
}

// -------- SILO --------
void leerSilo() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duracion = pulseIn(ECHO, HIGH, 30000);

  if (duracion == 0) {
    estadoSilo = "ERROR";
    return;
  }

  distancia = duracion * 0.034 / 2;
  nivel = (dist_max - distancia) * 100.0 / (dist_max - dist_min);

  if (nivel < 0) nivel = 0;
  if (nivel > 100) nivel = 100;

  if (nivel <= 10) {
    estadoSilo = "VACIO";
  } else if (nivel >= 90) {
    estadoSilo = "LLENO";
  } else {
    estadoSilo = String(nivel, 1) + "%";
  }
}

// -------- CLIMA --------
void leerClima() {
  hum = dht.readHumidity();
  temp = dht.readTemperature();

  if (isnan(hum) || isnan(temp)) {
    Serial.println("ERROR al leer DHT11");
    temp = -1;
    hum = -1;
  }
}

// -------- CONTROL --------
void controlClima() {
  if (temp > 24) {
    fanActivo = true;
    digitalWrite(FAN_LED, HIGH);

    int valorPot = analogRead(POT_PIN);
    int velocidad = map(valorPot, 0, 1023, 0, 255);
    analogWrite(MOTOR, velocidad);

  } else {
    fanActivo = false;
    digitalWrite(FAN_LED, LOW);
    analogWrite(MOTOR, 0);
  }
}

// -------- COMUNICACIÓN + LCD --------
void enviarDatos() {
  int velocidadActual = fanActivo ? map(analogRead(POT_PIN), 0, 1023, 0, 255) : 0;

  // ---- SERIAL ----
  Serial.print("SILO:");
  Serial.print(estadoSilo);
  Serial.print(",TEMP:");
  Serial.print(temp);
  Serial.print(",HUM:");
  Serial.print(hum);
  Serial.print(",FAN:");
  Serial.print(fanActivo ? 1 : 0);
  Serial.print(",MOTOR_VEL:");
  Serial.println(velocidadActual);

  // ---- LCD ----
  lcd.setCursor(0, 0);
  lcd.print("Silo:");
  lcd.print(estadoSilo);
  lcd.print("   "); // limpia restos

  lcd.setCursor(0, 1);
  lcd.print(temp);
  lcd.print("C ");

  if (fanActivo) {
    lcd.print("Fan ON ");
  } else {
    lcd.print("Fan OFF");
  }
}
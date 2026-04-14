#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11
#define FANPIN 3

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  pinMode(FANPIN, OUTPUT);
  dht.begin();
}

void loop() {
  // El DHT11 necesita al menos 1 segundo entre lecturas para no dar error
  delay(1000); 
  
  float t = dht.readTemperature();

  if (isnan(t)) {
    Serial.println("Error de lectura...");
    return;
  }

  Serial.print("Temp: ");
  Serial.print(t);
  Serial.print(" C | ");

  // Umbral bajado a 24 grados para probarlo fácil
  if (t > 24) { 
    digitalWrite(FANPIN, HIGH);
    Serial.println("VENTILADOR ACTIVADO");
  } else {
    digitalWrite(FANPIN, LOW);
    Serial.println("VENTILADOR APAGADO");
  }
}

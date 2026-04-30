#include <AccelStepper.h>
#include <Servo.h>

// ==============================
// STEPPER — orden de pines confirmado
// ==============================
#define PIN_IN1 8
#define PIN_IN2 9
#define PIN_IN3 10
#define PIN_IN4 11

AccelStepper stepper(AccelStepper::HALF4WIRE, PIN_IN1, PIN_IN3, PIN_IN2, PIN_IN4);

// ==============================
// SERVO
// ==============================
Servo servo;
const int servoPin = 6;

// ==============================
// VARIABLES DE ESTADO
// ==============================
int servoPos   = 90;
int stepperPos =  0;

// ==============================
// APAGAR BOBINAS (evita que el stepper caliente parado)
// ==============================
void apagarBobinas() {
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, LOW);
}

// ==============================
// SETUP
// ==============================
void setup() {
  Serial.begin(9600);

  // Stepper
  stepper.setMaxSpeed(1100.0);
  stepper.setAcceleration(1500.0);

  // Servo
  servo.attach(servoPin);
  servo.write(servoPos);

  Serial.println("Sistema listo.");
  Serial.println("Comandos: SERVO:90 | STEP:OPEN | STEP:CLOSE | STEP:%50 | STEP:STOP");
}

// ==============================
// LOOP PRINCIPAL
// ==============================
void loop() {

  // Ejecutar pasos pendientes del stepper en cada ciclo
  stepper.run();

  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toUpperCase();

    Serial.print("CMD: [");
    Serial.print(cmd);
    Serial.println("]");

    // -------------------------------------------------------
    // SERVO: SERVO:90  →  mueve el servo a 90°
    // -------------------------------------------------------
    if (cmd.startsWith("SERVO:")) {
      int angle = cmd.substring(6).toInt();
      angle = constrain(angle, 0, 180);
      servoPos = angle;
      servo.write(servoPos);
      Serial.print("Servo -> ");
      Serial.print(servoPos);
      Serial.println("°");
    }

    // -------------------------------------------------------
    // STEPPER: STEP:OPEN | STEP:CLOSE | STEP:%50 | STEP:STOP
    // -------------------------------------------------------
    else if (cmd.startsWith("STEP:")) {
      String action = cmd.substring(5);

      if (action == "OPEN") {
        stepper.move(512);
        stepperPos += 512;
        Serial.print("Stepper -> +512 pasos | Pos: ");
        Serial.println(stepperPos);
      }
      else if (action == "CLOSE") {
        stepper.move(-512);
        stepperPos -= 512;
        Serial.print("Stepper -> -512 pasos | Pos: ");
        Serial.println(stepperPos);
      }
      else if (action.startsWith("%")) {
        int percent = action.substring(1).toInt();
        percent = constrain(percent, 0, 100);
        int target = map(percent, 0, 100, 0, 2048);
        stepper.moveTo(target);
        stepperPos = target;
        Serial.print("Stepper -> ");
        Serial.print(percent);
        Serial.print("% | Pos: ");
        Serial.println(stepperPos);
      }
      else if (action == "STOP") {
        stepper.stop();
        apagarBobinas();
        Serial.println("Stepper detenido.");
      }
      else {
        Serial.print("Accion STEP desconocida: [");

        Serial.print(action);
        Serial.println("]");
      }
    }
    else {
      Serial.print("Comando no reconocido: [");
      Serial.print(cmd);
      Serial.println("]");
    }
  }
}
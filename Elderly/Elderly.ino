 te#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#define sensor A1

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *leftFrontMotor = AFMS.getMotor(2);
// You can also make another motor on port M2
Adafruit_DCMotor *rightFrontMotor = AFMS.getMotor(1);

Adafruit_DCMotor *leftBackMotor = AFMS.getMotor(3);
Adafruit_DCMotor *rightBackMotor = AFMS.getMotor(4);
void setup() {
  // put your setup code here, to run once:
  AFMS.begin(); // create with default frequency 1.6KHz or pass hz value in function
  pinMode(sensor, INPUT);
  Serial.begin(9600);
}

void loop() {
  int sensorval1 = analogRead(sensor);
  if (sensorval1 > 100) {
    Serial.println(sensorval1);
    delay(2000);
    forward(200);
    delay(1250);
    turnForward(200, 0);
    delay(900);
    forward(200);
    delay(850);
    forward(0);
    delay(30000);
  }

}

void forward(int i) {
  leftFrontMotor->setSpeed(i);
  leftFrontMotor->run(FORWARD);

  rightFrontMotor->setSpeed(i);
  rightFrontMotor->run(FORWARD);

  leftBackMotor->setSpeed(i);
  leftBackMotor->run(FORWARD);

  rightBackMotor->setSpeed(i);
  rightBackMotor->run(FORWARD);

}


void backward(int i) {
  leftFrontMotor->setSpeed(i);
  leftFrontMotor->run(BACKWARD);

  rightFrontMotor->setSpeed(i);
  rightFrontMotor->run(BACKWARD);

  leftBackMotor->setSpeed(i);
  leftBackMotor->run(BACKWARD);

  rightBackMotor->setSpeed(i);
  rightBackMotor->run(BACKWARD);

  

}

// when lm > rm, right turn
// when rm > lm, left turn
void turnForward(int lm, int rm) {
  leftFrontMotor->setSpeed(lm);
  leftFrontMotor->run(FORWARD);
  // turn on motor
  rightFrontMotor->setSpeed(rm);
  rightFrontMotor->run(FORWARD);
  leftBackMotor->setSpeed(lm);
  leftBackMotor->run(FORWARD);

  rightBackMotor->setSpeed(rm);
  rightBackMotor->run(FORWARD);
}

void turnBackward(int lm, int rm) {
  leftFrontMotor->setSpeed(lm);
  leftFrontMotor->run(BACKWARD);
  // turn on motor

  rightFrontMotor->setSpeed(rm);
  rightFrontMotor->run(BACKWARD);

  leftBackMotor->setSpeed(lm);
  leftBackMotor->run(BACKWARD);

  rightBackMotor->setSpeed(rm);
  rightBackMotor->run(BACKWARD);

}

/**
 * Code for the elderly robot
 * 
 * Use a sound sensor to know when to start the elderly. If the elderly hears
 * the siren for a certain period of time (use this to avoid noises), it will
 * start and run to safety based on a predefined path. 
 * 
 * Use Adafruit MotorShield to control the movement as well
 * 
 **/

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#define sensor A1

// Fix this to control the right wheels
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *leftFrontMotor = AFMS.getMotor(2);
Adafruit_DCMotor *rightFrontMotor = AFMS.getMotor(1);
Adafruit_DCMotor *leftBackMotor = AFMS.getMotor(3);
Adafruit_DCMotor *rightBackMotor = AFMS.getMotor(4);

void setup() {
  AFMS.begin();
  pinMode(sensor, INPUT);
}

void loop() {
  int sensorval1 = analogRead(sensor);
  
  // If the sound is loud enough, it is not noise and the elderly can move
  // towards safety.
  if (sensorval1 > 100) {
    // Run a pre-defined path
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

// Move straight forward
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

// Move straight backward
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

// Turn left or right forward
void turnForward(int lm, int rm) {
  leftFrontMotor->setSpeed(lm);
  leftFrontMotor->run(FORWARD);

  rightFrontMotor->setSpeed(rm);
  rightFrontMotor->run(FORWARD);
  
  leftBackMotor->setSpeed(lm);
  leftBackMotor->run(FORWARD);

  rightBackMotor->setSpeed(rm);
  rightBackMotor->run(FORWARD);
}

// Turn left or right backward
void turnBackward(int lm, int rm) {
  leftFrontMotor->setSpeed(lm);
  leftFrontMotor->run(BACKWARD);

  rightFrontMotor->setSpeed(rm);
  rightFrontMotor->run(BACKWARD);

  leftBackMotor->setSpeed(lm);
  leftBackMotor->run(BACKWARD);

  rightBackMotor->setSpeed(rm);
  rightBackMotor->run(BACKWARD);

}

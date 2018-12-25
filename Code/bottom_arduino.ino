/**
 * Code for the Arduino at the bottom
 * 
 * It will control the syringe system through 2 Servo motors where it will tilt 
 * the tube of the syringes up and down to pick up or distribute water
 * 
 **/

#include <Servo.h>
#define receiver 0
#define sLeft 9
#define sRight 10

Servo servoLeft;
Servo servoRight;

void setup() {
  pinMode(receiver, INPUT);

  servoLeft.attach(sLeft);
  servoRight.attach(sRight);

  // Tilting up position to start
  servoLeft.write(20);
  servoRight.write(160);
  delay(5000);
}

void loop() {
  /**
  Tilting down when the signal is HIGH
  Tilting up when the signal is LOW
  **/
  if (digitalRead(receiver) == HIGH) {
    servoLeft.write(150);
    servoRight.write(30);
  } else {
    servoLeft.write(45);
    servoRight.write(135);
  }

}

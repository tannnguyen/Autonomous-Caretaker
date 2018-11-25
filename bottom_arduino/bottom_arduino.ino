#include <Servo.h>
#define receiver 0
#define sLeft 9
#define sRight 10

Servo servoLeft;
Servo servoRight;

void setup() {
  // put your setup code here, to run once:
  pinMode(receiver, INPUT);

  servoLeft.attach(sLeft);
  servoRight.attach(sRight);
  // Tilting up position to start
  servoLeft.write(20);
  servoRight.write(160);
  delay(5000);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(digitalRead(receiver));
  // Code explanation:
  // Tilting down when the signal is HIGH
  // Tilting up when the signal is LOW
  if (digitalRead(receiver) == HIGH) {
    servoLeft.write(150);
    servoRight.write(30);
  } else {
    servoLeft.write(45);
    servoRight.write(135);
  }

}

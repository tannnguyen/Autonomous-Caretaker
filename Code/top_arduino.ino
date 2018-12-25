/**
 * Code for the main control system of the robot
 * 
 * It will control almost everything in the 2-story robot and execute the tasks
 * steps by steps. Refer to the README documents for the steps and the comments
 * below to see the execution works
 **/

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

// Include headers for SPI port and pixy camera
#include <SPI.h>
#include <Pixy.h>

// Define all speeds and delay here
#define water 3000
#define hose 500
#define rotateSpeed 200

// Define all ports here
#define siren 3
#define signalPin 2
#define ping 7
#define enA 45
#define enB 44
#define in1 43
#define in2 41
#define in3 42
#define in4 40

/*
   Decode for motors:
   Left syringe is 45 43 41 with 41 HIGH means Pull
   Right syringe is 44 42 40 with 42 HIGH means Pull
*/

// Create the motor shield object
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *leftFrontMotor = AFMS.getMotor(3);
Adafruit_DCMotor *rightFrontMotor = AFMS.getMotor(2);
Adafruit_DCMotor *rightBackMotor = AFMS.getMotor(1);
Adafruit_DCMotor *leftBackMotor = AFMS.getMotor(4);

// Initialize Pixy object
Pixy pixy;

//put all the variables here!
int color_fire = -1;
int color_elderly = -1;
int color_water = -1;
uint16_t blocks;
int x = -1;
int area = -1;
int height = -1;

void setup() {
  // Initialize variables here
  AFMS.begin();
  pixy.init();
  pinMode(siren, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(signalPin, OUTPUT);
  digitalWrite(signalPin, LOW);
  // Set the constant speed of the syringe
  analogWrite(enA, 225);
  analogWrite(enB, 235);
  delay(5000);
}

void loop() {
  /*
     Challenge 1 code right here
     Step 1: Looking for the fire (red)
     Step 2: Once found, looking for the elderly
     Step 3: Move towards the elderly. Trying to center it
     Step 4: Turn on the siren
     Step 5: Move backward and look for the water
  */

  // Step 1: Looking for red
  color_fire = -1;
  while (color_fire == -1) {
    rotateLeft(rotateSpeed);
    blocks = pixy.getBlocks();
    // The blocks need to be red. Signature 1.
    if (blocks) {
      for (int j = 0; j < blocks; j++) {
        if (pixy.blocks[j].signature == 1) {
          color_fire = j;
        }
      }
    }
  }
  // Step 1.5: Checking for the ping to make sure that we are allowed to go
  long distance = measureDistance();
  // Check if we will get stuck as we find the elderly or not
  if (distance < 70) {
    //Looking for the water to center itself first
    color_water = -1;
    while (color_water == -1) {
      rotateLeft(rotateSpeed);
      blocks = pixy.getBlocks();
      if (blocks) {
        for (int j = 0; j < blocks; j++) {
          if (pixy.blocks[j].signature == 2) {
            color_water = j;
            area = pixy.blocks[j].height * pixy.blocks[j].width;
            x = pixy.blocks[j].x;
          }
        }
      }
    }
    // Trying to center the robot with the water
    while (x < 160) {
      rotateRight(130);
      blocks = pixy.getBlocks();
      if (blocks) {
        for (int j = 0; j < blocks; j++) {
          // If there is noise in the environment, check for the largest object
          if (pixy.blocks[j].signature == 2) {
            x = pixy.blocks[j].x;
          }
        }
      }
    }
    while (distance < 70) {
      forward(200);
      distance = measureDistance();
    }
  }
  // Step 2: Found red here, looking for the elderly next
  int x = -1;
  int height = 0;
  //find the elderly and turn to it
  color_elderly = -1;
  while (color_elderly == -1) {
    // Need to pan to search for the robot
    rotateRight(rotateSpeed);
    // Use Pixy to search for the robot
    blocks = pixy.getBlocks();
    // The blocks need to be green. Signature 3.
    if (blocks) {
      for (int j = 0; j < blocks; j++) {
        if (pixy.blocks[j].signature == 3) {
          color_elderly = j;
          height = pixy.blocks[j].height;
          x = pixy.blocks[j].x;
        }
      }
    }
  }

  // Stop the robot and ready to center with the elderly.
  forward(0);
  delay(50);
  // Step 3: Move towards the elderly (at a 12in distance)
  while (height < 80) {
    if (x < 160) {
      //elderly on the left of rescue robot
      turnForward(100, 190);
    } else {
      //elderly on the right of rescue robot
      turnForward(190, 100);
    }
    blocks = pixy.getBlocks();
    if (blocks) {
      color_elderly = -1;
      for (int j = 0; j < blocks; j++) {
        // If there is noise in the environment, check for the largest object
        if (pixy.blocks[j].signature == 3) {
          color_elderly = j;
          height = pixy.blocks[j].height;
          x = pixy.blocks[j].x;
        }
      }
    }
  }

  //Stop the robot
  forward(0);
  delay(50);
  // Step 4: Turn on the siren
  digitalWrite(siren, HIGH);
  delay(3000);
  digitalWrite(siren, LOW);
  // Step 5: Move backward
  backward(200);
  delay(500);

  /*
    Challenge 2:
      Step 1: Identify where the water is
      Step 2: Move to the water.
      Step 3: Pick up the water. (Send the signal to the other Arduino)
      Step 4: Move back a bit, look for the fire
      Step 5: Go to the fire.
      Step 6: Distribute the water (send the signal to the Arduino)
      Step 7: Go to the elderly.
      Step 8: Distribute the water to the elderly.
  */

  // Step 1: Identify where the water is
  area = -1;
  x = -1;
  color_water = -1;
  rotateLeft(rotateSpeed);
  delay(1000);
  while (color_water == -1) {
    rotateLeft(rotateSpeed);
    blocks = pixy.getBlocks();
    if (blocks) {
      for (int j = 0; j < blocks; j++) {
        if (pixy.blocks[j].signature == 2) {
          color_water = j;
          area = pixy.blocks[j].height * pixy.blocks[j].width;
          x = pixy.blocks[j].x;
        }
      }
    }
  }
  // Found the water here.
  // Step 2: Move towards the water
  // Stop at certain area. Need to change this threshold here!
  while (area < 10000) {
    if (x < 160) {
      //water on the left of the robot
      turnForward(100, 190);
    } else {
      //water on the right of rescue robot
      turnForward(190, 100);
    }
    blocks = pixy.getBlocks();
    if (blocks) {
      for (int j = 0; j < blocks; j++) {
        // If there is noise in the environment, check for the largest object
        if (pixy.blocks[j].signature == 2) {
          area = pixy.blocks[j].width * pixy.blocks[j].height;
          x = pixy.blocks[j].x;
        }
      }
    }
  }
  // Should stop at the water area here.
  // Move to the edge of the water.
  forward(0);
  delay(50);
  turnForward(200, 170);
  delay(1000);
  forward(0);

  // Step 3: Pick up the water
  pullWater();

  // Step 4: Move back and look for fire
  backward(200);
  delay(800);
  // Look around for the fire
  height = 0;
  color_fire = -1;
  while (color_fire == -1) {
    // Need to pan to search for the robot
    rotateRight(rotateSpeed);
    // Use Pixy to search for the robot
    blocks = pixy.getBlocks();
    // The blocks need to be red. Signature 1.
    if (blocks) {
      for (int j = 0; j < blocks; j++) {
        // If there is noise in the environment, check for the largest object
        if (pixy.blocks[j].signature == 1) {
          color_fire = j;
          height = pixy.blocks[j].height;
          x = pixy.blocks[j].x;
        }
      }
    }
  }
  // Found the fire here
  // Step 5: Go to the fire
  while (height < 110) {
    if (x < 140) {
      //elderly on the left of rescue robot
      turnForward(100, 150);
    } else if (x >= 140 && x < 180) {
      turnForward(120, 120);
    } else {
      //elderly on the right of rescue robot
      turnForward(150, 100);
    }
    blocks = pixy.getBlocks();
    if (blocks) {
      for (int j = 0; j < blocks; j++) {
        // If there is noise in the environment, check for the largest object
        if (pixy.blocks[j].signature == 1) {
          height = pixy.blocks[j].height;
          x = pixy.blocks[j].x;
        }
      }
    }
  }
  // In the fire area. Go to the fire.
  forward(0);
  delay(100);
  forward(200);
  delay(200);
  forward(0);

  while (x > 100) {
    rotateLeft(130);
    blocks = pixy.getBlocks();
    if (blocks) {
      for (int j = 0; j < blocks; j++) {
        // If there is noise in the environment, check for the largest object
        if (pixy.blocks[j].signature == 1) {
          x = pixy.blocks[j].x;
        }
      }
    }

  }
  // Step 6: Distribute the water to the fire
  forward(0);
  pushLeft();


  // Step 7: Look for the elderly
  backward(100);
  delay(500);
  forward(0);
  height = 0;
  color_elderly = -1;
  while (color_elderly == -1) {
    // Need to pan to search for the robot
    rotateRight(rotateSpeed);
    // Use Pixy to search for the robot
    blocks = pixy.getBlocks();
    // The blocks need to be green. Signature 3.
    if (blocks) {
      for (int j = 0; j < blocks; j++) {
        // If there is noise in the environment, check for the largest object
        if (pixy.blocks[j].signature == 3) {
          color_elderly = j;
          height = pixy.blocks[j].height;
          x = pixy.blocks[j].x;
        }
      }
    }
  }

  // Step 8: Go to the elderly
  forward(0);
  delay(50);
  while (height < 80) {
    if (x < 150) {
      //elderly on the left of rescue robot
      turnForward(100, 150);
    } else if (x >= 150 && x < 170) {
      turnForward(120, 120);
    } else {
      //elderly on the right of rescue robot
      turnForward(150, 100);
    }
    blocks = pixy.getBlocks();
    if (blocks) {
      for (int j = 0; j < blocks; j++) {
        // If there is noise in the environment, check for the largest object
        if (pixy.blocks[j].signature == 3) {
          color_elderly = j;
          height = pixy.blocks[j].height;
          x = pixy.blocks[j].x;
        }
      }
    }
  }
  // Got to the elderly
  // Step 9: Distribute the water to the elderly
  forward(0);
  delay(50);
  forward(100);
  delay(300);
  while (x < 150) {
    rotateRight(130);
    blocks = pixy.getBlocks();
    if (blocks) {
      for (int j = 0; j < blocks; j++) {
        // If there is noise in the environment, check for the largest object
        if (pixy.blocks[j].signature == 1) {
          x = pixy.blocks[j].x;
        }
      }
    }
  }

  forward(0);
  pushRight();
  backward(200);
  delay(100);

  // Stop everything
  forward(0);
  delay(100000000);
}

// Move forward
void forward(int i) {
  leftFrontMotor->setSpeed(i);
  leftFrontMotor->run(FORWARD);
  leftBackMotor->setSpeed(i);
  leftBackMotor->run(FORWARD);
  rightBackMotor->setSpeed(i);
  rightBackMotor->run(FORWARD);
  rightFrontMotor->setSpeed(i);
  rightFrontMotor->run(FORWARD);
}

// Move backward
void backward(int i) {
  leftFrontMotor->setSpeed(i);
  leftFrontMotor->run(BACKWARD);
  leftBackMotor->setSpeed(i);
  leftBackMotor->run(BACKWARD);
  rightBackMotor->setSpeed(i);
  rightBackMotor->run(BACKWARD);
  rightFrontMotor->setSpeed(i);
  rightFrontMotor->run(BACKWARD);
}

// Turn left or right forward
void turnForward(int lm, int rm) {
  leftFrontMotor->setSpeed(lm);
  leftFrontMotor->run(FORWARD);
  leftBackMotor->setSpeed(lm);
  leftBackMotor->run(FORWARD);
  rightBackMotor->setSpeed(rm);
  rightBackMotor->run(FORWARD);
  rightFrontMotor->setSpeed(rm);
  rightFrontMotor->run(FORWARD);
}

// Turn left or right backward
void turnBackward(int lm, int rm) {
  leftFrontMotor->setSpeed(lm);
  leftFrontMotor->run(BACKWARD);
  leftBackMotor->setSpeed(lm);
  leftBackMotor->run(BACKWARD);
  rightBackMotor->setSpeed(rm);
  rightBackMotor->run(BACKWARD);
  rightFrontMotor->setSpeed(rm);
  rightFrontMotor->run(BACKWARD);

}

// Rotate around to the left
void rotateLeft(int i) {
  leftFrontMotor->setSpeed(i);
  leftFrontMotor->run(FORWARD);
  leftBackMotor->setSpeed(i);
  leftBackMotor->run(FORWARD);
  rightBackMotor->setSpeed(i);
  rightBackMotor->run(BACKWARD);
  rightFrontMotor->setSpeed(i);
  rightFrontMotor->run(BACKWARD);

}

// Rotate around to the right
void rotateRight(int i) {
  leftFrontMotor->setSpeed(i);
  leftFrontMotor->run(BACKWARD);
  leftBackMotor->setSpeed(i);
  leftBackMotor->run(BACKWARD);
  rightBackMotor->setSpeed(i);
  rightBackMotor->run(FORWARD);
  rightFrontMotor->setSpeed(i);
  rightFrontMotor->run(FORWARD);
}

// Control the motor to suck in water
void pullWater() {
  // Servo so the hose goes down
  digitalWrite(signalPin, HIGH);
  delay(hose);
  // Pull water in. Adjust the delay for the amount
  digitalWrite(in4, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in2, HIGH);
  digitalWrite(in1, LOW);
  delay(water);

  // Stop pulling water now
  digitalWrite(in4, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in1, LOW);
  //Put it to the top
  digitalWrite(signalPin, LOW);
}

// Control the left servo to push water out
void pushLeft() {
  // Push the left motor out
  analogWrite(enB, 200);
  analogWrite(enA, 200);
  digitalWrite(in1, HIGH);
  delay(water * 1.5);

  // Stop pushing now
  digitalWrite(in1, LOW);
  // Put servo up
}

// Control the right servo to push water out
void pushRight() {
  // Push the right motor out
  digitalWrite(in4, HIGH);
  delay(water);
  
  // Stop pushing now
  digitalWrite(in4, LOW);
}

//For each ultrasound sensor pin to send the signal
long measureDistance() {
  long duration, cm;
  pinMode(ping, OUTPUT);
  digitalWrite(ping, LOW);
  delayMicroseconds(5);
  digitalWrite(ping, HIGH);
  delayMicroseconds(5);
  digitalWrite(ping, LOW);
  pinMode(ping, INPUT);
  duration = pulseIn(pin, HIGH);
  cm = microseconds / 29 / 2;
  return cm;
}


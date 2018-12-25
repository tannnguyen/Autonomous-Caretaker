# Autonomous Caretaker

Autonomous Caretaker is the final project for class 530.420 Mechatronics in Spring 2017 at Johns Hopkins University. As a team of engineers, we built a futuristic robot that can recognize and put off fire and then rescue another robot on its way back to base. We want to share our results (including Arduino codes, 3D design, and circuit).

## Mission
Given a hypothetical situation in the future where everyone has a supporting robot with them. An elderly human is sleeping on the beach and forgets to put off the campfire. The robot needs to recognize the fire, alert the elders to wake up, collect the water, put off the fire, and then bring some water back to the elderly. 

## Approaches
For our robot, we plan to build a 2-story robot where it can store water in a syringe system at the bottom and all the control system is on top. We use 2 Arduinos, each controls a story within the robot. To support this robot, we use a 4-wheel robot for stability, Pixy for color and object recognition, various types of motors to control different moving components in the robot.

Our steps to solve the problems:

    Milestone 1: 
    Step 1: Looking for the fire (registered by the color red)
    Step 2: Once found, looking for the elderly
    Step 3: Move towards the elderly
    Step 4: Once reach the elderly, turn on the siren
    Step 4.5 (for elderly): Elderly hears the siren and move to safety

    Milestone 2:
    Step 5: Move backward and look for the water (registered by the color blue)
    Step 6: Identify where the water is
    Step 7: Move to the water.
    Step 8: Pick up the water (send signal to bottom Arduino)
    Step 9: Move back a bit, look for the fire
    Step 10: Go to the fire.
    Step 11: Distribute the water (send the signal to the bottom Arduino)
    Step 12: Go to the elderly.
    Step 13: Distribute the water to the elderly.
  
## Code, Design, and Circuit
Please refer to Code/ and Parts/ folders to see and download our codes and design. Refer to Parts.md for our parts

This is our mechanical design:

This is our circuit:

This is our final robot:


## Demo
* Challenge 1:

* Challenge 2:

* We did not have a chance to take video of our final demo with complete steps...

## Technology
* Ardunio and its libraries
* SolidWorks (for 3D printed and lasercut part)
* Mechanical and electrical components needed to build the robot

## Authors
* Tan Nguyen
* Darshini Balamurugan
* Mona Gao

## Acknowledgement
* A lot of help and support from Morris Kracer, Ethan Tang, Joseph Pan, and Paola Donis
* A lot of guidance from Professor Charbel Rizk
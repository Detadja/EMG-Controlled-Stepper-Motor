# EMG-Controlled-Stepper-Motor
This code will be uploaded onto an Arduino to collect EMG data to control a stepper motor.

## Description
* This setup and code will collect EMG data from two points corresponding to a flexor and extensor muscle (any). 
* For the first five seconds, once the program runs, the LED will be turned off and it will read the baseline EMG, as such the user must relax both muscles. 
* Afterwards, the LED will start flickering for five seconds, giving ample time for the user to contract both muscles to their maximum. 
* Once both these steps are done, the LED will turn off for 1.5 seconds and turn on fully. This is an indication to the user that they can now use the setup and control the direction and speed of the stepper motor using their muscles.

## Below is a diagram of the circuit used and setup
[![Circuit Diagram](https://github.com/Detadja/EMG-Controlled-Stepper-Motor/blob/main/EMG%20Stepper%20Circuit.png)](https://github.com/Detadja)
[![Setup Diagram 1](https://github.com/Detadja/EMG-Controlled-Stepper-Motor/blob/main/EMG%20Stepper%20Setup%201.jpg)](https://github.com/Detadja)
[![Setup Diagram 1](https://github.com/Detadja/EMG-Controlled-Stepper-Motor/blob/main/EMG%20Stepper%20Setup%202.jpg)](https://github.com/Detadja)
**NOTE: It is recommended that hair from body parts be shaved, before use for the EMG collection.**

## Materials Used:
* 1x Arduino UNO board
* 2x MyoWare Muscle Sensor 1.0
* 1x LED
* 1x 10 uF Capacitor
* 1x 220 Ohm Resistor
* 6x EMG Electrode Patches
* 1x 28BYJ-48 4-Phase Stepper Motor
* 1x ULN2003 Driver
* Male-to-Female Jumper Wires

## Files
* **EMG_Stepper** - File includes the dynamic thresholding and direction discrimination features.
* **EMG_Stepper_Simplified** - File includes a set thresholding and direction discrimination features.
* **EMG_Stepper_simple** - File includes no thresholding and only direction discrimination feature.

## To Run
Clone the repo to a local repo on your computer. Compile and run on the Arduino IDE, while connected to the Arduino UNO.

## Demo
[Demo](https://github.com/Detadja/EMG-Controlled-Stepper-Motor/blob/main/EMG%20Stepper%20Demo.mp4)

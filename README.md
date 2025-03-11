# EMG-Controlled-Stepper-Motor
This code will be uploaded onto an Arduino to collect EMG data to control a stepper motor.

## Description
This setup and code will collect EMG data from two points corresponding to a flexor and extensor muscle (any). For the first five seconds, once the program runs, the LED will be turned off and it will read the baseline EMG, as such the user must relax both muscles. Afterwards, the LED will start flickering for five seconds, giving ample time for the user to contract both muscles to their maximum. Once both these steps are done, the LED will turn off for 1.5 seconds and turn on fully. This is an indication to the user that they can now use the setup and control the direction and speed of the stepper motor using their muscles.

## Below is a diagram of the circuit used and setup
[<img src="path/to/image.png">](https://github.com/Detadja/EMG-Controlled-Stepper-Motor/blob/main/EMG%20Stepper%20Circuit.png)
[INSERT SETUP PHOTO HERE]

## Materials Used:
* 1 Arduino UNO board
* 2 MyoWare Muscle Sensor 1.0
* 1 LED
* 1 10 uF Capacitor
* 1 220 Ohm Resistor
* 6 EMG Electrode Patches
* 1 28BYJ-48 4-Phase Stepper Motor
* 1 ULN2003 Driver
* Male-to-Female Jumper Wires

## To Run
Clone the repo to a local repo on your computer. Compile and run on the Arduino IDE, while connected to the Arduino UNO.

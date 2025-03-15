# EMG-Controlled-Stepper-Motor
This code will be uploaded onto an Arduino to collect EMG data to control a stepper motor.

## Description
### EMG_Stepper
* This setup and code will collect EMG data from two points corresponding to a flexor and extensor muscle (any) and threshold the user's baseline and maximum EMG values to dynamically control the stepper motor rotational speed, as well as direction discrimination. 
* For the first five seconds, once the program runs, the LED will be turned off, and it will read the baseline EMG; as such, the user must relax both muscles. 
* Afterwards, the LED will start flickering for five seconds, giving ample time for the user to contract both muscles to their maximum. 
* Once both these steps are done, the LED will turn off for 1.5 seconds and turn on fully. This is an indication to the user that they can now use the setup and control the direction and speed of the stepper motor using their muscles.
### EMG_Stepper_Simplified
* Similar to EMG_Stepper, this setup and code will collect EMG data from two points on the body.
* Unlike it, the user's baseline and maximum EMG values are set manually to control the stepper motor's rotational speed, but values obtained from readings will be used for direction discrimination.
* Immediately after the program runs, it will start collecting EMG data until one second's worth of data is read.
* The LED within this duration is turned off.
* Afterwards, the LED will turn on entirely. This is an indication to the user that they can now use the setup and control the direction and speed of the stepper motor using their muscles.
### EMG_Stepper_Simple
* Similar to EMG_Stepper and EMG_Stepper_Simplified, this setup and code will collect EMG data from two points on the body.
* Immediately after the program runs, it will start collecting EMG data until one second's worth of data is read.
* The LED within this duration is turned off.
* Afterwards, the LED will turn on entirely. This is an indication to the user that they can now use the setup and control the direction of the stepper motor using their muscles.

## Files
* **EMG_Stepper** - The file includes the dynamic thresholding and direction discrimination features.
* **EMG_Stepper_Simplified** - The file includes a set thresholding and direction discrimination features.
* **EMG_Stepper_simple** - The file includes no thresholding and only a direction discrimination feature.

### Key Variables
* **emg_interval** - Determines the sampling rate of EMG recording.
* **motor_max** - Determines the maximum speed of the stepper motor (Recommended absolute value of 1, at ~29 rpm).
* **motor_min** - Determines the minimum speed of the stepper motor (Recommended absolute minimum of 50, at ~15 rpm).
* **baseline** - Determines the baseline or minimum EMG value set by the user.
* **emg_max** - Determines the maximum amg value set by the user.

## Below is a diagram of the circuit used and setup
[![Circuit Diagram](https://github.com/Detadja/EMG-Controlled-Stepper-Motor/blob/main/EMG%20Stepper%20Circuit.png)](https://github.com/Detadja)
[![Setup Diagram 1](https://github.com/Detadja/EMG-Controlled-Stepper-Motor/blob/main/EMG%20Stepper%20Setup%201.jpg)](https://github.com/Detadja)
[![Setup Diagram 1](https://github.com/Detadja/EMG-Controlled-Stepper-Motor/blob/main/EMG%20Stepper%20Setup%202.jpg)](https://github.com/Detadja)
**NOTE: It is recommended that hair from body parts be shaved** <ins>**before**</ins> **placing electrodes for EMG collection.**

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

## To Run
Clone the repo to a local repo on your computer. Compile and run on the Arduino IDE while connected to the Arduino UNO.

## Demo
### EMG_Stepper_Simplified
[Demo](https://github.com/Detadja/EMG-Controlled-Stepper-Motor/blob/main/EMG%20Stepper%20Demo.mp4)

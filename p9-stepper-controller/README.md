# Project 9: Stepper Motor Precision Positioner

## Overview

This project demonstrates precise position control of a stepper motor using an Arduino Uno. The target angle is selected using a potentiometer, and three push buttons are used to rotate the motor clockwise, counter-clockwise, and set the home position. The OLED display shows the current angle, target angle, motor direction, and total motor steps.

The project was designed and tested using the Wokwi online simulator.

## Components Used

- Arduino Uno R3
- Stepper Motor
- Potentiometer
- Push Buttons ×3
- OLED Display 0.96" I2C

## Wokwi Connections

### Stepper Motor

- Pin 8 → Stepper Input 1
- Pin 9 → Stepper Input 2
- Pin 10 → Stepper Input 3
- Pin 11 → Stepper Input 4

### Potentiometer

- VCC → 5V
- GND → GND
- Signal → A0

### Push Buttons

- Clockwise Button → Pin 2
- Counter-Clockwise Button → Pin 3
- Home Button → Pin 4

### OLED Display

- SDA → A4
- SCL → A5
- VCC → 5V
- GND → GND

## Working

- The potentiometer sets the target angle between 0° and 360°.
- The Clockwise button rotates the motor by 45°.
- The Counter-Clockwise button rotates the motor by 45° in the opposite direction.
- The Home button resets the current position to 0°.
- The OLED continuously displays:
  - Current Angle
  - Target Angle
  - Direction
  - Step Count
- The Serial Monitor displays the current motor position and direction.

## Expected Output

- Stepper motor rotates according to the button input.
- Potentiometer changes the target angle.
- OLED displays current angle, target angle, direction, and steps.
- Serial Monitor continuously displays motor position.

## Software Used

- Arduino IDE
- Wokwi Online Simulator

## Libraries Used

- Wire.h
- Adafruit GFX
- Adafruit SSD1306
- Stepper.h

## Wokwi Simulation

This project was created and tested using the Wokwi online simulator.

**Wokwi Project Link:**
(https://wokwi.com/projects/470225858315692033)

## Demo Video
https://drive.google.com/file/d/1Z_ZN1NHj6lAORhzCuZehk6HcA_65MX-J/view?usp=sharing

## Note

This project was developed and verified using the Wokwi simulator. The simulation demonstrates the complete functionality of the stepper motor position controller, including potentiometer input, button controls, OLED display, and Serial Monitor output.

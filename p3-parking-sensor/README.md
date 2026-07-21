# Project 3 - Ultrasonic Parking Assistant

## Objective

Build a parking assistance system using an HC-SR04 ultrasonic sensor. The system detects the distance of nearby objects and alerts the user using LEDs, a buzzer, and an OLED display.

---

## Components Used

- Arduino Uno R3
- HC-SR04 Ultrasonic Sensor
- Active Buzzer
- Red LED
- Yellow LED
- Green LED
- 220Ω Resistors
- OLED Display (0.96" I2C)
- Breadboard
- Jumper Wires

---

## Circuit Connections

### HC-SR04

| Sensor | Arduino |
|---------|----------|
| VCC | 5V |
| GND | GND |
| TRIG | D9 |
| ECHO | D10 |

### LEDs

| LED | Arduino |
|------|----------|
| Green | D2 |
| Yellow | D3 |
| Red | D4 |

(All LEDs connected through 220Ω resistors.)

### Buzzer

Pin D6

### OLED

| OLED | Arduino |
|-------|----------|
| VCC | 5V |
| GND | GND |
| SDA | A4 |
| SCL | A5 |

---

## Working Principle

The ultrasonic sensor sends an ultrasonic pulse and waits for the echo. The distance is calculated using:

Distance = (Time × Speed of Sound) / 2

Formula:

distance = pulseIn(ECHO, HIGH) × 0.034 / 2

Five readings are averaged to reduce measurement noise.

---

## Distance Zones

| Distance | Zone | LED | Buzzer |
|-----------|------|------|---------|
| >60 cm | SAFE | Green | OFF |
| 30–60 cm | CAUTION | Yellow | Beep every 800 ms |
| 15–30 cm | CLOSE | Yellow + Red | Beep every 300 ms |
| <15 cm | DANGER | Red | Continuous Beep |

---

## Features

- Distance measurement using HC-SR04
- Noise reduction using averaging
- Four warning zones
- LED indication
- Buzzer alert
- OLED distance display
- Serial Monitor output
- Non-blocking timing using millis()

---

## Output

- OLED displays distance in centimeters
- Serial Monitor displays distance and zone
- LEDs indicate warning level
- Buzzer frequency increases as object approaches

## Demo Video

https://drive.google.com/file/d/1N9ede1irJJcmi-1vH39ncnD_gDzynx8Y/view?usp=sharing

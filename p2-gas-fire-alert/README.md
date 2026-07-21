# Gas & Fire Safety Alert System

## IIT Jammu Summer School 2026 – Home Assignment Project 2

### Project Overview
The Gas & Fire Safety Alert System is an Arduino-based safety project that detects gas leaks and fire hazards in real time. It uses an MQ-2 Gas Sensor to measure gas concentration and a Flame Sensor to detect fire. Based on the detected conditions, the system provides three alert levels using LEDs and a buzzer.

## Objective

- Detect LPG gas leakage and smoke using the MQ-2 sensor.
- Detect fire using the flame sensor.
- Display SAFE, WARNING, or DANGER status through LEDs and buzzer.
- Print the system status to the Serial Monitor every second.
- Improve home and kitchen safety by providing early alerts.

## Components Used

- Arduino Uno R3
- MQ-2 Gas Sensor
- Flame Sensor
- Active Buzzer
- Green LED
- Yellow LED
- Red LED
- 220Ω Resistors
- Breadboard
- Jumper Wires
- USB Cable

## Wiring Diagram (Text)

### MQ-2 Gas Sensor

| MQ-2 Pin | Arduino Pin |
|----------|-------------|
| VCC | 5V |
| GND | GND |
| AOUT | A0 |

> **Note:** The MQ-2 sensor requires **1–2 minutes of warm-up time** after powering on before the readings become stable.

### Flame Sensor

| Flame Sensor Pin | Arduino Pin |
|-----------------|-------------|
| VCC | 5V |
| GND | GND |
| DO | D7 |
| AO | A1 (Optional) |

> **Note:** The flame sensor is **active-low**, which means it outputs **LOW** when a flame is detected.

### LEDs

| LED | Arduino Pin |
|-----|-------------|
| Green LED | D10 |
| Yellow LED | D11 |
| Red LED | D12 |

### Buzzer

| Buzzer Pin | Arduino Pin |
|------------|-------------|
| Positive (+) | D9 |
| Negative (-) | GND |

## Software Used

- Arduino IDE
- GitHub

## Project Working

1. The MQ-2 sensor continuously measures gas concentration.
2. The gas value is converted into a percentage (0–100%).
3. The flame sensor checks for the presence of fire.
4. The system operates in three states:

### SAFE
- Gas level below 30%
- No flame detected
- Green LED ON
- Buzzer OFF

### WARNING
- Gas level between 30% and 60%
- Yellow LED ON
- Slow beep (1 beep per second)

### DANGER
- Gas level above 60% OR flame detected
- Red LED ON
- Continuous buzzer alarm

If both gas leakage and flame are detected simultaneously, the system remains in the **DANGER** state.


## Alert Levels

| Gas Level | Flame | Status |
|-----------|--------|--------|
| Less than 30% | No | SAFE |
| 30% – 60% | No | WARNING |
| Greater than 60% | No | DANGER |
| Any Gas Level | Flame Detected | DANGER |

## Expected Serial Monitor Output

```text
GAS: 18% | FLAME: NONE | STATUS: SAFE
GAS: 45% | FLAME: NONE | STATUS: WARNING
GAS: 72% | FLAME: NONE | STATUS: DANGER
GAS: 35% | FLAME: DETECTED | STATUS: DANGER
```

## Expected Output

### SAFE
- Green LED ON
- Yellow LED OFF
- Red LED OFF
- Buzzer OFF

### WARNING
- Yellow LED ON
- Slow beep (1 beep/second)

### DANGER
- Red LED ON
- Continuous buzzer alarm

## Folder Structure

```text
home-assignments/
└── p2-gas-fire-alert/
    ├── gas_fire_alert.ino
    ├── README.md
    └── images/
        ├── serial_output.png
        └── demo.jpg
```

## Demo

The project demonstrates real-time gas leak and fire detection using Arduino Uno, MQ-2 Gas Sensor, and Flame Sensor. LEDs and buzzer provide visual and audible alerts based on the detected hazard level.


## Author

**Rubadharshini S**  
Electronics and Communication Engineering (ECE)  
Arjun College of Technology

## License

This project was developed for **IIT Jammu Summer School 2026 – Home Assignment** for educational purposes.

## Demo Video
[
https://drive.google.com/file/d/1sA8XWHVbynvHaasb9Z5hassLgZA3KZQ2/view?usp=sharing

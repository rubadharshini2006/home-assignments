# Project 4 - Smart Plant Watering Monitor

## Objective

Build an automated plant watering monitor using ESP32. The system measures soil moisture and environmental conditions, automatically activates a water pump (simulated using a relay and LED), and displays information on an OLED display.

---

## Components Used

- ESP32 Dev Board
- Capacitive Soil Moisture Sensor
- DHT11 Temperature & Humidity Sensor
- 2-Channel Relay Module
- OLED Display (0.96" I2C)
- Red LED
- Green LED
- Active Buzzer
- Push Button
- Breadboard
- Jumper Wires

---

## Circuit Connections

### Soil Moisture Sensor

| Sensor | ESP32 |
|---------|--------|
| VCC | 3.3V |
| GND | GND |
| AOUT | GPIO34 |

### DHT11

| Pin | ESP32 |
|-----|--------|
| VCC | 3.3V |
| GND | GND |
| DATA | GPIO4 |

### Relay Module

| Relay | ESP32 |
|--------|--------|
| VCC | 5V |
| GND | GND |
| IN1 | GPIO26 |
| IN2 | GPIO27 |

### Push Button

GPIO0 → Button → GND

### OLED Display

| OLED | ESP32 |
|------|--------|
| SDA | GPIO21 |
| SCL | GPIO22 |
| VCC | 3.3V |
| GND | GND |

---

## Working Principle

The ESP32 continuously reads the soil moisture sensor and DHT11 sensor.

The soil moisture sensor is calibrated using:

- Dry value = 3200
- Wet value = 1200

The raw ADC value is converted into a moisture percentage using:

```
Moisture % = map(rawValue, 3200, 1200, 0, 100)
```

The relay simulates a water pump.

Pump ON when moisture < 30%.

Pump OFF only when moisture > 40%.

This hysteresis prevents frequent ON/OFF switching.

The OLED displays:

- Soil Moisture
- Temperature
- Humidity
- Pump Status

A push button allows manual watering for 5 seconds.

---

## Soil Status

| Moisture | Status |
|-----------|---------|
| <30% | DRY |
| 30–70% | OPTIMAL |
| >70% | WET |

---

## Features

- Soil moisture monitoring
- Temperature monitoring
- Humidity monitoring
- Automatic watering
- Manual override button
- OLED Display
- Relay Control
- Hysteresis Logic
- Serial Monitor Output

---

## Calibration Values

Dry Value : 3200

Wet Value : 1200

(Replace with your own measured values.)

---

## Output

OLED shows:

Soil: 45%

Temp: 27°C

Humidity: 60%

Status: OPTIMAL

Pump: OFF

## Demo Video

https://drive.google.com/file/d/1_xC1Cfw3tYt4cnggr2VIdj-pulbriuYg/view?usp=sharing

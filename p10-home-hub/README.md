# Project 10: Full IoT Home Automation Hub

## Overview

This project is a complete IoT-based Home Automation Hub developed using the ESP32. It combines multiple sensors and automation features into a single system. The project monitors temperature, humidity, gas level, room brightness, and motion detection. Based on these sensor readings, the system automatically controls a fan and a light using relay modules. The project also provides a web dashboard and publishes sensor data to an MQTT broker for remote monitoring.

## Components Used

- ESP32 Dev Board
- DHT11 Temperature & Humidity Sensor
- MQ-2 Gas Sensor
- PIR Motion Sensor (HC-SR501)
- LDR (Light Dependent Resistor)
- 2-Channel Relay Module
- OLED Display 0.96" I2C
- Active Buzzer
- Red, Green and Yellow LEDs
- Push Buttons ×2

## Circuit Connections

### DHT11 Sensor
- VCC → 3.3V
- GND → GND
- DATA → GPIO4

### MQ-2 Gas Sensor
- VCC → 5V
- GND → GND
- AOUT → GPIO35

### PIR Motion Sensor
- VCC → 5V
- GND → GND
- OUT → GPIO13

### LDR Sensor
- One terminal → 3.3V
- Other terminal → GPIO34
- 10kΩ resistor from GPIO34 to GND

### Relay Module
- Relay 1 (Fan) → GPIO26
- Relay 2 (Light) → GPIO27
- VCC → 5V
- GND → GND

### OLED Display
- SDA → GPIO21
- SCL → GPIO22
- VCC → 3.3V
- GND → GND

### Buzzer
- Positive → GPIO14
- Negative → GND

### LEDs
- Red LED → GPIO25
- Green LED → GPIO2
- Yellow LED → GPIO33

### Push Buttons
- Fan Override → GPIO0
- Light Override → GPIO32

## System Architecture

```
Sensors
│
├── DHT11
├── MQ-2
├── PIR
└── LDR
      │
      ▼
      ESP32
      │
      ├── Relay 1 (Fan)
      ├── Relay 2 (Light)
      ├── OLED Display
      ├── Buzzer
      ├── LEDs
      ├── Web Dashboard
      └── MQTT Broker
```

## Automation Rules

### Fan Control

- Fan turns ON automatically when temperature is above **32°C**.
- Fan turns OFF automatically when temperature falls below **28°C**.
- Manual override is available for 10 minutes.

### Light Control

- Light turns ON when:
  - LDR detects darkness.
  - PIR detects motion.
- Light turns OFF when:
  - Daylight is detected.
  - No motion is detected for 3 minutes.

### Gas Alert

- If gas level exceeds **60%**:
  - Red LED turns ON.
  - Buzzer sounds.
  - All relays turn OFF.
  - Alert message is published to MQTT.

## OLED Display

The OLED cycles through three screens every 5 seconds.

### Screen 1
- Temperature
- Humidity

### Screen 2
- Gas Level
- Motion Status
- Light Level

### Screen 3
- Fan Status
- Light Status
- System Uptime

## MQTT Topic

```
iitjammu/yourname/home
```

Example JSON Message

```json
{
  "temp": 29,
  "humidity": 62,
  "gas": 18,
  "pir": 0,
  "light": 72,
  "fan": "OFF",
  "light_relay": "ON",
  "alert": "SAFE"
}
```

## Web Dashboard Features

- Live Temperature
- Humidity
- Gas Level
- Motion Detection
- Light Level
- Fan Status
- Light Status
- Manual Relay Control
- System Uptime

## Expected Output

- Displays all sensor readings on OLED.
- Controls fan and light automatically.
- Sends gas alerts through MQTT.
- Displays sensor values on a web dashboard.
- Shows relay status and system uptime.

## Software Used

- Arduino IDE
- ESP32 Board Package
- MQTT Explorer
- Web Browser

## Libraries Used

- WiFi.h
- ESPAsyncWebServer.h
- AsyncTCP.h
- PubSubClient.h
- DHTesp.h
- Adafruit GFX
- Adafruit SSD1306
- Wire.h

## Deliverables

- Arduino Source Code (.ino)
- README.md
- Web Dashboard Screenshot
- MQTT Explorer Screenshot
- Serial Monitor Screenshot
- Demo Video

## Demo Video

https://drive.google.com/file/d/1Qk4jQcowOFme_DNXeogH6zXifXAXQYBi/view?usp=sharing

## Notes

This project combines environmental monitoring, home automation, web-based monitoring, and MQTT communication into one integrated IoT system. It demonstrates real-time sensing, automation, remote monitoring, and safety features using the ESP32.

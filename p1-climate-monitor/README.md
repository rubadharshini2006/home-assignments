# Smart Room Climate Monitor

## IIT Jammu Summer School 2026 – Home Assignment Project 1

### Project Overview
The Smart Room Climate Monitor is an IoT-based project developed using the ESP32. It continuously monitors the room temperature and humidity using a DHT11 sensor. The measured values are displayed on an OLED screen, and the system alerts the user when the room becomes too hot or humid using a red LED and an active buzzer.

---

## Objective
- Monitor room temperature and humidity in real time.
- Display sensor readings on an OLED display.
- Alert the user when temperature exceeds **38°C** or humidity exceeds **80%**.
- Log sensor data to the Serial Monitor in CSV format.

---

## Components Used

- ESP32 Dev Board
- DHT11 Temperature & Humidity Sensor
- 0.96" OLED Display (SSD1306 I2C)
- Active Buzzer
- Red LED
- Green LED
- 220Ω Resistors
- Breadboard
- Jumper Wires
- USB Cable

---

## Wiring Diagram (Text)

### DHT11
| DHT11 Pin | ESP32 Pin |
|-----------|-----------|
| VCC | 3.3V |
| GND | GND |
| DATA | GPIO4 |

> A 10kΩ pull-up resistor is connected between DATA and 3.3V.

### OLED Display
| OLED Pin | ESP32 Pin |
|----------|-----------|
| VCC | 3.3V |
| GND | GND |
| SDA | GPIO21 |
| SCL | GPIO22 |

### LEDs
| Component | ESP32 Pin |
|-----------|-----------|
| Red LED | GPIO25 |
| Green LED | GPIO26 |

### Buzzer
| Buzzer Pin | ESP32 Pin |
|------------|-----------|
| Positive (+) | GPIO27 |
| Negative (-) | GND |

---

## Software Used

- Arduino IDE
- ESP32 Board Package
- GitHub

---

## Libraries Used

- DHTesp
- Adafruit GFX Library
- Adafruit SSD1306 Library
- Wire Library

---

## Project Working

1. The ESP32 reads temperature and humidity from the DHT11 sensor every **2 seconds**.
2. The readings are displayed on the OLED display.
3. The system classifies the room into one of four conditions:
   - COOL
   - COMFORT
   - HOT
   - DANGER
4. If the temperature exceeds **38°C** or humidity exceeds **80%**:
   - Red LED turns ON.
   - Green LED turns OFF.
   - Buzzer sounds for 1 second.
5. Otherwise:
   - Green LED remains ON.
   - Red LED remains OFF.
6. Every **5 seconds**, the ESP32 prints the sensor data to the Serial Monitor in CSV format.

---

## Comfort Levels

| Temperature | Humidity | Status |
|-------------|----------|--------|
| 25°C – 33°C | 40% – 69% | COMFORT |
| ≥34°C or ≥70% | - | HOT |
| >38°C or >80% | - | DANGER |
| Otherwise | - | COOL |

---

## Expected OLED Output

```
Temp: 29.5 C
Hum : 58 %

Status: COMFORT
```

---

## Expected Serial Monitor Output

```
Temperature: 29.5 C    Humidity: 58 %

5000,29.5,58,COMFORT
10000,30.1,60,COMFORT
15000,35.4,72,HOT
20000,39.2,84,DANGER
25000,28.8,56,COMFORT
```

---

## Folder Structure

```
home-assignments/
└── p1-climate-monitor/
    ├── weather.ino
    ├── README.md
    └── images/
        ├── serial_output.png
        └── oled_display.jpg
```

---

## Demo

The project demonstrates real-time room climate monitoring using ESP32. Temperature and humidity are displayed on the OLED screen, while LEDs and a buzzer provide alerts whenever the environment becomes uncomfortable or unsafe.

---

## Author

**Rubadharshini S**  
Electronics and Communication Engineering (ECE)  
Arjun College of Technology

---

## License

This project is developed for **IIT Jammu Summer School 2026 – Home Assignment** for educational purposes.

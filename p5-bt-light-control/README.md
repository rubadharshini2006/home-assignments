# Bluetooth Home Light Controller

## IIT Jammu Summer School 2026 – Home Assignment Project 5

### Project Overview
The Bluetooth Home Light Controller is an ESP32-based home automation project that allows users to control two lights wirelessly using Bluetooth. An Android phone sends simple commands through a Bluetooth Terminal app, and the ESP32 switches two relay channels accordingly. The system also provides manual control using push buttons, confirmation beeps through a buzzer, and automatically turns off all lights after 30 minutes of inactivity to save energy.


## Objective

- Control two lights wirelessly using Bluetooth.
- Provide manual control using push buttons.
- Give confirmation through a buzzer after every valid command.
- Display the current light status on the Bluetooth Terminal.
- Automatically turn OFF all lights after 30 minutes without Bluetooth activity.

## Components Used

- ESP32 Dev Board
- 2-Channel Relay Module
- Active Buzzer
- Push Buttons (2)
- LEDs (for light simulation)
- 220Ω Resistors
- Breadboard
- Jumper Wires
- Android Phone with Bluetooth Terminal App

## Wiring Diagram (Text)

### Relay Module

| Relay Pin | ESP32 Pin |
|-----------|-----------|
| IN1 | GPIO26 |
| IN2 | GPIO27 |
| VCC | 5V (VIN) |
| GND | GND |


### Push Buttons

| Button | ESP32 Pin |
|---------|-----------|
| Button 1 | GPIO0 |
| Button 2 | GPIO35 |


### Buzzer

| Buzzer Pin | ESP32 Pin |
|------------|-----------|
| Positive (+) | GPIO25 |
| Negative (-) | GND |


### Bluetooth Status LED

| LED | ESP32 Pin |
|-----|-----------|
| Built-in LED | GPIO2 |

## Software Used

- Arduino IDE
- ESP32 Board Package
- BluetoothSerial Library
- GitHub

## Bluetooth Pairing Instructions

1. Upload the program to the ESP32.
2. Turn on Bluetooth on your Android phone.
3. Pair with the ESP32 device named:

```
IIT_IoT_HomeCtrl
```

4. Open the **Serial Bluetooth Terminal** app.
5. Connect to the paired ESP32 device.
6. Send commands to control the lights.

## Supported Bluetooth Commands

| Command | Action |
|----------|--------|
| 1 | Light 1 ON |
| 2 | Light 1 OFF |
| 3 | Light 2 ON |
| 4 | Light 2 OFF |
| 5 | Turn ON Both Lights |
| 6 | Turn OFF Both Lights |
| ? | Show Current Status |

## Project Working

1. ESP32 waits for Bluetooth commands from the mobile phone.
2. When a valid command is received:
   - Relay state changes.
   - Buzzer beeps for 50 ms.
   - Current light status is sent back through Bluetooth.
3. Manual push buttons can also control each light.
4. The built-in LED on GPIO2 indicates Bluetooth connection status.
5. If no Bluetooth command is received for 30 minutes, both lights turn OFF automatically.

## Expected Bluetooth Output

```
Command : 1
Response: L1:ON L2:OFF

Command : 3
Response: L1:ON L2:ON

Command : 6
Response: L1:OFF L2:OFF

Command : ?
Response: L1:OFF L2:OFF
```

## Expected Output

- Bluetooth successfully connects to the ESP32.
- Relay 1 and Relay 2 operate according to received commands.
- Buzzer beeps after every valid command.
- Push buttons manually toggle the lights.
- Bluetooth Terminal displays the current light status.
- Built-in LED glows while Bluetooth is connected.
- Both lights automatically switch OFF after 30 minutes of inactivity.

## Folder Structure

```text
home-assignments/
└── p5-bt-light-control/
    ├── bt_light_controller.ino
    ├── README.md
    └── images/
        ├── bluetooth_terminal.png
        └── demo.jpg
```

## Demo

This project demonstrates Bluetooth-based wireless control of two home lights using an ESP32. Lights can be controlled from an Android phone or manually using push buttons. The system provides visual and audible feedback and includes an automatic timeout feature for energy saving.

## Author

**Rubadharshini S**  
Electronics and Communication Engineering (ECE)  
Arjun College of Technology

## License

This project was developed for **IIT Jammu Summer School 2026 – Home Assignment** for educational purposes.

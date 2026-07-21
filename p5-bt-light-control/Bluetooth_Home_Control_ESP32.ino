/*******************
  Bluetooth Home Control using ESP32

  Board: ESP32 Dev Board
  Bluetooth Name: IIT_IoT_HomeCtrl

  Commands:
  '1' -> Light1 ON
  '2' -> Light1 OFF
  '3' -> Light2 ON
  '4' -> Light2 OFF
  '5' -> ALL ON
  '6' -> ALL OFF
  '?' -> Status
*******************/

#include <BluetoothSerial.h>

BluetoothSerial btLink;

const uint8_t PIN_RELAY_LIGHT1 = 26;
const uint8_t PIN_RELAY_LIGHT2 = 27;

const uint8_t PIN_SWITCH1 = 0;   
const uint8_t PIN_SWITCH2 = 35;

const uint8_t PIN_BUZZER  = 25;
const uint8_t PIN_STATUS_LED = 2;   

const uint8_t RELAY_ENGAGE   = LOW;
const uint8_t RELAY_RELEASE  = HIGH;

bool light1On = false;
bool light2On = false;

unsigned long lastCommandTime = 0;
const unsigned long IDLE_SHUTOFF_MS = 1800000UL; 

int switch1Prev = LOW;
int switch2Prev = LOW;

void setup()
{
  Serial.begin(115200);

  btLink.begin("IIT_IoT_HomeCtrl");

  pinMode(PIN_RELAY_LIGHT1, OUTPUT);
  pinMode(PIN_RELAY_LIGHT2, OUTPUT);

  pinMode(PIN_SWITCH1, INPUT_PULLUP);
  pinMode(PIN_SWITCH2, INPUT);

  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_STATUS_LED, OUTPUT);

  digitalWrite(PIN_RELAY_LIGHT1, RELAY_RELEASE);
  digitalWrite(PIN_RELAY_LIGHT2, RELAY_RELEASE);
  digitalWrite(PIN_BUZZER, LOW);
  digitalWrite(PIN_STATUS_LED, LOW);

  Serial.println("Bluetooth Home Controller Ready");
}

void loop()
{
  showConnectionState();
  handleBluetoothCommand();
  handleSwitch1();
  handleSwitch2();
  handleIdleShutoff();
}


void showConnectionState()
{
  digitalWrite(PIN_STATUS_LED, btLink.hasClient() ? HIGH : LOW);
}

void handleBluetoothCommand()
{
  if (!btLink.available())
    return;

  char incoming = btLink.read();
  lastCommandTime = millis();

  bool validCommand = true;

  switch (incoming)
  {
    case '1': light1On = true;  break;
    case '2': light1On = false; break;
    case '3': light2On = true;  break;
    case '4': light2On = false; break;

    case '5':
      light1On = true;
      light2On = true;
      break;

    case '6':
      light1On = false;
      light2On = false;
      break;

    case '?':
      reportStatus();
      return;

    default:
      btLink.println("Invalid Command");
      return;
  }

  applyRelayStates();
  chirp();
  reportStatus();
}

void handleSwitch1()
{
  int reading = digitalRead(PIN_SWITCH1);

  if (switch1Prev == HIGH && reading == LOW)
  {
    light1On = !light1On;

    applyRelayStates();
    chirp();
    reportStatus();

    delay(200);
  }

  switch1Prev = reading;
}

void handleSwitch2()
{
  int reading = digitalRead(PIN_SWITCH2);

  if (switch2Prev == LOW && reading == HIGH)
  {
    light2On = !light2On;

    applyRelayStates();
    chirp();
    reportStatus();

    delay(200);
  }

  switch2Prev = reading;
}

void handleIdleShutoff()
{
  if (millis() - lastCommandTime > IDLE_SHUTOFF_MS)
  {
    light1On = false;
    light2On = false;

    applyRelayStates();
  }
}

void applyRelayStates()
{
  digitalWrite(PIN_RELAY_LIGHT1, light1On ? RELAY_ENGAGE : RELAY_RELEASE);
  digitalWrite(PIN_RELAY_LIGHT2, light2On ? RELAY_ENGAGE : RELAY_RELEASE);
}

void chirp()
{
  digitalWrite(PIN_BUZZER, HIGH);
  delay(50);
  digitalWrite(PIN_BUZZER, LOW);
}

void reportStatus()
{
  String line = "";

  line += "L1:";
  line += (light1On ? "ON " : "OFF ");

  line += "L2:";
  line += (light2On ? "ON" : "OFF");

  Serial.println(line);
  btLink.println(line);
}

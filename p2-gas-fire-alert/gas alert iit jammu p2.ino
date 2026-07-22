/********************************************************
   Kitchen Safety Alert System
   Board  : Arduino Uno R3
   Sensor : MQ-2 Gas Sensor + Flame Sensor
   Output : Green LED, Yellow LED, Red LED, Buzzer

   Connections:
   MQ-2 AOUT  -> A0
   Flame DO   -> D7
   Flame AO   -> A1 (Optional)
   Green LED  -> D10
   Yellow LED -> D11
   Red LED    -> D12
   Buzzer     -> D9
********************************************************/

const int PIN_GAS_SENSOR   = A0;
const int PIN_FLAME_DIGITAL = 7;

const int PIN_LED_GREEN  = 10;
const int PIN_LED_YELLOW = 11;
const int PIN_LED_RED    = 12;
const int PIN_BUZZER     = 9;

enum AlertLevel
{
  LEVEL_SAFE,
  LEVEL_WARNING,
  LEVEL_DANGER,
  LEVEL_FLAME,
  LEVEL_GAS_AND_FLAME
};

unsigned long beepToggleAt = 0;
unsigned long lastPrintAt  = 0;
bool buzzerOn = false;

void setup()
{
  Serial.begin(9600);

  pinMode(PIN_FLAME_DIGITAL, INPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_YELLOW, OUTPUT);
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  digitalWrite(PIN_LED_GREEN, LOW);
  digitalWrite(PIN_LED_YELLOW, LOW);
  digitalWrite(PIN_LED_RED, LOW);
  digitalWrite(PIN_BUZZER, LOW);

  Serial.println("======================================");
  Serial.println(" Kitchen Safety Alert System");
  Serial.println(" MQ-2 Warming Up...");
  Serial.println(" Wait 2 Minutes...");
  Serial.println("======================================");

  delay(120000);   // 2 minute sensor warm-up

  Serial.println("MQ-2 Ready!");
}

void loop()
{
  int gasPercent = readGasPercent();
  bool flameSeen = isFlameDetected();

  AlertLevel level = evaluateAlertLevel(gasPercent, flameSeen);

  driveOutputs(level);
  printStatusEvery1Sec(gasPercent, flameSeen, level);
}

int readGasPercent()
{
  int rawValue = analogRead(PIN_GAS_SENSOR);
  return map(rawValue, 0, 1023, 0, 100);
}

bool isFlameDetected()
{
  // Flame sensor pulls the line LOW when flame is present
  return digitalRead(PIN_FLAME_DIGITAL) == LOW;
}

AlertLevel evaluateAlertLevel(int gasPercent, bool flameSeen)
{
  if (gasPercent > 60 && flameSeen)
    return LEVEL_GAS_AND_FLAME;

  if (flameSeen)
    return LEVEL_FLAME;

  if (gasPercent > 60)
    return LEVEL_DANGER;

  if (gasPercent > 30)
    return LEVEL_WARNING;

  return LEVEL_SAFE;
}

const char* alertLevelName(AlertLevel level)
{
  switch (level)
  {
    case LEVEL_SAFE:          return "SAFE";
    case LEVEL_WARNING:       return "WARNING";
    case LEVEL_DANGER:        return "DANGER";
    case LEVEL_FLAME:         return "FLAME";
    case LEVEL_GAS_AND_FLAME: return "GAS + FLAME";
  }
  return "SAFE";
}

void driveOutputs(AlertLevel level)
{
  switch (level)
  {
    case LEVEL_SAFE:
      digitalWrite(PIN_LED_GREEN, HIGH);
      digitalWrite(PIN_LED_YELLOW, LOW);
      digitalWrite(PIN_LED_RED, LOW);
      digitalWrite(PIN_BUZZER, LOW);
      break;

    case LEVEL_WARNING:
      digitalWrite(PIN_LED_GREEN, LOW);
      digitalWrite(PIN_LED_YELLOW, HIGH);
      digitalWrite(PIN_LED_RED, LOW);
      pulseBuzzer(500);   // slow beep, ~1 beep/sec
      break;

    case LEVEL_DANGER:
      digitalWrite(PIN_LED_GREEN, LOW);
      digitalWrite(PIN_LED_YELLOW, LOW);
      digitalWrite(PIN_LED_RED, HIGH);
      digitalWrite(PIN_BUZZER, HIGH);   // continuous alarm
      break;

    case LEVEL_FLAME:
      digitalWrite(PIN_LED_GREEN, LOW);
      digitalWrite(PIN_LED_YELLOW, LOW);
      digitalWrite(PIN_LED_RED, HIGH);
      pulseBuzzer(150);   // fast beep pattern
      break;

    case LEVEL_GAS_AND_FLAME:
      digitalWrite(PIN_LED_GREEN, LOW);
      digitalWrite(PIN_LED_YELLOW, HIGH);
      digitalWrite(PIN_LED_RED, HIGH);
      pulseBuzzer(80);    // combined rapid pattern
      break;
  }
}

void pulseBuzzer(unsigned long intervalMs)
{
  if (millis() - beepToggleAt >= intervalMs)
  {
    beepToggleAt = millis();
    buzzerOn = !buzzerOn;
    digitalWrite(PIN_BUZZER, buzzerOn);
  }
}

void printStatusEvery1Sec(int gasPercent, bool flameSeen, AlertLevel level)
{
  if (millis() - lastPrintAt < 1000)
    return;

  lastPrintAt = millis();

  Serial.print("GAS: ");
  Serial.print(gasPercent);
  Serial.print("%");
  Serial.print(" | FLAME: ");
  Serial.print(flameSeen ? "DETECTED" : "NONE");
  Serial.print(" | STATUS: ");
  Serial.println(alertLevelName(level));
}

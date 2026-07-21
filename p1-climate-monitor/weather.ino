/******************************************************
  ESP32 Mini Weather Station 
******************************************************/

#include <Wire.h>
#include <DHTesp.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const uint8_t DHTPIN    = 4;
const uint8_t RED_LED   = 25;
const uint8_t GREEN_LED = 26;
const uint8_t BUZZER    = 27;

DHTesp dht;

const unsigned long SENSOR_INTERVAL = 2000UL;
const unsigned long CSV_INTERVAL    = 5000UL;
unsigned long prevSensorMillis = 0;
unsigned long prevCsvMillis    = 0;

float temperature = 0.0f;
float humidity = 0.0f;
String statusText = "COMFORT";

void setupPins() {
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(BUZZER, LOW);
}

void initOLED() {
  Wire.begin(21, 22);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found!");
    while (true) delay(1000);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Mini Weather");
  display.println("Station");
  display.display();
  delay(2000);
}

void readSensor() {
  TempAndHumidity d = dht.getTempAndHumidity();
  temperature = d.temperature;
  humidity = d.humidity;
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" C\tHumidity: ");
  Serial.print(humidity);
  Serial.println(" %");
}

void evaluateComfort() {
  if (temperature > 38.0f || humidity > 80.0f) {
    statusText = "DANGER";
  } else if (temperature >= 34.0f || humidity >= 70.0f) {
    statusText = "HOT";
  } else if (temperature >= 25.0f && temperature <= 33.0f &&
             humidity >= 40.0f && humidity <= 69.0f) {
    statusText = "COMFORT";
  } else {
    statusText = "COOL";
  }
}

void updateDisplay() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Temp: ");
  display.print(temperature, 1);
  display.println(" C");
  display.setCursor(0,20);
  display.print("Hum : ");
  display.print(humidity, 0);
  display.println(" %");
  display.setCursor(0,40);
  display.print("Status: ");
  display.println(statusText);
  display.display();
}

void updateOutputs() {
  if (temperature > 38.0f || humidity > 80.0f) {
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    // buzzer beep for 1 second without blocking major timing:
    digitalWrite(BUZZER, HIGH);
    delay(1000);
    digitalWrite(BUZZER, LOW);
  } else {
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(BUZZER, LOW);
  }
}

void logCSV() {
  Serial.print(millis());
  Serial.print(",");
  Serial.print(temperature);
  Serial.print(",");
  Serial.print(humidity);
  Serial.print(",");
  Serial.println(statusText);
}

void setup() {
  Serial.begin(115200);
  dht.setup(DHTPIN, DHTesp::DHT11);
  setupPins();
  initOLED();
}

void loop() {
  unsigned long now = millis();

  if (now - prevSensorMillis >= SENSOR_INTERVAL) {
    prevSensorMillis = now;
    readSensor();
    evaluateComfort();
    updateDisplay();
    updateOutputs();
  }

  if (now - prevCsvMillis >= CSV_INTERVAL) {
    prevCsvMillis = now;
    logCSV();
  }
}
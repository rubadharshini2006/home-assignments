/*******************
  Smart Plant Watering Monitor
  IIT Jammu Summer School 2026
*******************/

#include <Wire.h>
#include <DHTesp.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
DHTesp dht;

#define SOIL_PIN     34
#define DHT_PIN      4
#define RELAY1       26
#define RELAY2       27
#define BUTTON       13
#define RED_LED      25
#define GREEN_LED    33
#define BUZZER       32

const int DRY_VALUE = 3200;
const int WET_VALUE = 1200;

int soilPercent = 0;
float temperature = 0;
float humidity = 0;

bool pumpOn = false;
bool manualMode = false;

unsigned long lastRead = 0;
unsigned long manualStart = 0;

const unsigned long SENSOR_INTERVAL = 5000;  

String statusText = "OPTIMAL";

void setup()
{
  Serial.begin(115200);

  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  pinMode(BUZZER, OUTPUT);

  pinMode(BUTTON, INPUT_PULLUP);

  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);

  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);

  dht.setup(DHT_PIN, DHTesp::DHT11);

  Wire.begin(21,22);

  if(!display.begin(SSD1306_SWITCHCAPVCC,0x3C))
  {
    Serial.println("OLED Failed");
    while(true);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  display.setCursor(15,20);
  display.println("Plant Monitor");
  display.display();

  delay(2000);
}

void loop()
{
  if(millis()-lastRead>=SENSOR_INTERVAL)
  {
    lastRead=millis();

    readSensors();

    automaticControl();

    updateDisplay();

    printSerial();
  }

  checkButton();

  manualWatering();
}

void readSensors()
{
  int raw = analogRead(SOIL_PIN);

  soilPercent = map(raw,DRY_VALUE,WET_VALUE,0,100);
  soilPercent = constrain(soilPercent,0,100);

  TempAndHumidity data = dht.getTempAndHumidity();

  temperature = data.temperature;
  humidity = data.humidity;
}

void automaticControl()
{
  if(manualMode) return;

  if(!pumpOn && soilPercent<30)
  {
    pumpOn=true;
  }

  if(pumpOn && soilPercent>40)
  {
    pumpOn=false;
  }

  digitalWrite(RELAY1,pumpOn?LOW:HIGH);

  if(soilPercent<30)
  {
    statusText="DRY";

    digitalWrite(RED_LED,HIGH);
    digitalWrite(GREEN_LED,LOW);

    digitalWrite(BUZZER,HIGH);
  }
  else if(soilPercent<=70)
  {
    statusText="OPTIMAL";

    digitalWrite(RED_LED,LOW);
    digitalWrite(GREEN_LED,HIGH);

    digitalWrite(BUZZER,LOW);
  }
  else
  {
    statusText="WET";

    digitalWrite(RED_LED,LOW);
    digitalWrite(GREEN_LED,HIGH);

    digitalWrite(BUZZER,LOW);
  }
}

void checkButton()
{
  static bool lastState=HIGH;

  bool current=digitalRead(BUTTON);

  if(lastState==HIGH && current==LOW)
  {
    manualMode=true;
    manualStart=millis();

    digitalWrite(RELAY1,LOW);

    Serial.println("Manual Watering Started");

    delay(200);
  }

  lastState=current;
}


void manualWatering()
{
  if(manualMode)
  {
    if(millis()-manualStart>=5000)
    {
      manualMode=false;

      digitalWrite(RELAY1,HIGH);

      Serial.println("Manual Watering Finished");
    }
  }
}

void updateDisplay()
{
  display.clearDisplay();

  display.setCursor(0,0);
  display.print("Soil:");
  display.print(soilPercent);
  display.println("%");

  display.print("Temp:");
  display.print(temperature,1);
  display.println(" C");

  display.print("Status:");
  display.println(statusText);

  display.drawRect(5,52,100,8,WHITE);

  int bar=map(soilPercent,0,100,0,98);

  display.fillRect(6,53,bar,6,WHITE);

  display.display();
}

void printSerial()
{
  Serial.print("SOIL:");
  Serial.print(soilPercent);
  Serial.print("% | ");

  Serial.print("TEMP:");
  Serial.print(temperature);

  Serial.print(" C | ");

  Serial.print("HUM:");
  Serial.print(humidity);

  Serial.print("% | ");

  Serial.print("STATUS:");

  Serial.println(statusText);
}
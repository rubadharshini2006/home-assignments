#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Stepper.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Stepper
const int STEPS_PER_REV = 2048;
Stepper motor(STEPS_PER_REV, 8, 10, 9, 11);

// Pins
const int potPin = A0;
const int cwBtn = 2;
const int ccwBtn = 3;
const int homeBtn = 4;
const int ledPin = 7;

long currentSteps = 0;
String direction = "STOP";

void setup() {

  Serial.begin(9600);

  pinMode(cwBtn, INPUT_PULLUP);
  pinMode(ccwBtn, INPUT_PULLUP);
  pinMode(homeBtn, INPUT_PULLUP);

  pinMode(ledPin, OUTPUT);

  motor.setSpeed(10);

  if(!display.begin(SSD1306_SWITCHCAPVCC,0x3C)){
    while(true);
  }

  display.clearDisplay();
  display.display();
}

void loop() {

  int pot = analogRead(potPin);

  int targetAngle = map(pot,0,1023,0,360);

  float currentAngle = fmod(currentSteps * 360.0 / STEPS_PER_REV,360);

  if(currentAngle < 0)
    currentAngle += 360;

  // CW Button
  if(digitalRead(cwBtn)==LOW){

    motor.step(256);

    currentSteps += 256;

    direction = "CW";

    delay(250);
  }

  // CCW Button
  if(digitalRead(ccwBtn)==LOW){

    motor.step(-256);

    currentSteps -= 256;

    direction = "CCW";

    delay(250);
  }

  // HOME Button
  if(digitalRead(homeBtn)==LOW){

    currentSteps = 0;

    direction = "HOME";

    delay(250);
  }

  digitalWrite(ledPin, HIGH);

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0,0);
  display.print("Current:");
  display.print((int)currentAngle);
  display.println(" deg");

  display.print("Target :");
  display.print(targetAngle);
  display.println(" deg");

  display.print("Steps  :");
  display.println(currentSteps);

  display.print("Dir    :");
  display.println(direction);

  display.display();

  Serial.print("Current Angle : ");
  Serial.print(currentAngle);

  Serial.print(" | Target : ");
  Serial.print(targetAngle);

  Serial.print(" | Steps : ");
  Serial.print(currentSteps);

  Serial.print(" | Direction : ");
  Serial.println(direction);

  delay(100);
}

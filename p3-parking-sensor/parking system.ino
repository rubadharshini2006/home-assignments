/*
  Project : Ultrasonic Parking Assistant
  Author  : Rubadharshini S
*/

const int trigPin = 9;
const int echoPin = 10;

const int greenLED = 2;
const int yellowLED = 3;
const int redLED = 4;

const int buzzer = 6;

unsigned long lastBeep = 0;
unsigned long lastPrint = 0;

float measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);

  return duration * 0.034 / 2.0;
}

float averageDistance() {
  float sum = 0;

  for (int i = 0; i < 5; i++) {
    sum += measureDistance();
    delayMicroseconds(500);
  }

  return sum / 5.0;
}

void setup() {

  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);

  pinMode(buzzer, OUTPUT);
}

void loop() {

  float distance = averageDistance();

  unsigned long currentTime = millis();

  String zone;

  digitalWrite(greenLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(redLED, LOW);

  noTone(buzzer);

  if (distance > 60) {

    zone = "SAFE";

    digitalWrite(greenLED, HIGH);
  }

  else if (distance > 30) {

    zone = "CAUTION";

    digitalWrite(yellowLED, HIGH);

    if (currentTime - lastBeep >= 800) {
      tone(buzzer, 1000, 100);
      lastBeep = currentTime;
    }
  }

  else if (distance > 15) {

    zone = "CLOSE";

    digitalWrite(yellowLED, HIGH);
    digitalWrite(redLED, HIGH);

    if (currentTime - lastBeep >= 300) {
      tone(buzzer, 1000, 100);
      lastBeep = currentTime;
    }
  }

  else {

    zone = "DANGER";

    digitalWrite(redLED, HIGH);

    tone(buzzer, 1000);
  }

  if (currentTime - lastPrint >= 500) {

    Serial.print("Distance : ");
    Serial.print(distance);
    Serial.print(" cm");

    Serial.print("    Zone : ");
    Serial.println(zone);

    lastPrint = currentTime;
  }
}

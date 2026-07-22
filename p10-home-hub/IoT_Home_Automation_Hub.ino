/****************************************************
   FULL IoT HOME AUTOMATION HUB
   ESP32 - IIT Jammu Summer School 2026

   Sensors:
   DHT11
   MQ-2
   PIR
   LDR

   Outputs:
   2-Channel Relay
   OLED
   Buzzer
   Red LED
   Green LED

   No Manual Buttons
****************************************************/

#include <WiFi.h>
#include <WebServer.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// ================= WIFI =================

const char* ssid = "ECE-LAB-01 1161";
const char* password = "123456789";

// ================= MQTT =================

const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient mqttClient(espClient);

String mqttTopic = "iitjammu/RUBADHARSHINI/home";

// ================= PIN DEFINITIONS =================

// DHT11
#define DHTPIN 4
#define DHTTYPE DHT11

// Sensors
#define MQ2_PIN 35
#define PIR_PIN 13
#define LDR_PIN 34

// Relay
#define FAN_RELAY 26
#define LIGHT_RELAY 27

// Alert
#define BUZZER_PIN 14
#define RED_LED 25
#define GREEN_LED 2

// OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_SDA 21
#define OLED_SCL 22

// ================= OBJECTS =================

DHT dht(DHTPIN, DHTTYPE);

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  -1
);

WebServer server(80);

// ================= GLOBAL VARIABLES =================

float temperature = 0;
float humidity = 0;

int gasValue = 0;
int gasPercent = 0;

int ldrValue = 0;
int lightPercent = 0;

int pirState = 0;

bool fanState = false;
bool lightState = false;
bool gasAlert = false;

unsigned long lastSensorRead = 0;
unsigned long lastMQTTPublish = 0;
unsigned long lastOLEDUpdate = 0;

int oledScreen = 0;

// Light motion timer
unsigned long lastMotionTime = 0;

// ================= SETUP =================

void setup()
{
  Serial.begin(115200);

  // Pin modes
  pinMode(MQ2_PIN, INPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);

  pinMode(FAN_RELAY, OUTPUT);
  pinMode(LIGHT_RELAY, OUTPUT);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  // Relay OFF initially
  digitalWrite(FAN_RELAY, HIGH);
  digitalWrite(LIGHT_RELAY, HIGH);

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);

  // DHT start
  dht.begin();

  // OLED start
  Wire.begin(OLED_SDA, OLED_SCL);

  if (!display.begin(
        SSD1306_SWITCHCAPVCC,
        0x3C
      ))
  {
    Serial.println("OLED not found");
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  display.setCursor(0, 0);
  display.println("IoT HOME HUB");
  display.println("Starting...");
  display.display();

  delay(2000);

  // Wi-Fi
  connectWiFi();

  // MQTT
  mqttClient.setServer(
    mqtt_server,
    1883
  );

  // Web routes
  server.on("/", handleRoot);

  server.on(
    "/fan/on",
    []()
    {
      fanState = true;
      digitalWrite(FAN_RELAY, LOW);
      server.send(200, "text/plain", "Fan ON");
    }
  );

  server.on(
    "/fan/off",
    []()
    {
      fanState = false;
      digitalWrite(FAN_RELAY, HIGH);
      server.send(200, "text/plain", "Fan OFF");
    }
  );

  server.on(
    "/light/on",
    []()
    {
      lightState = true;
      digitalWrite(LIGHT_RELAY, LOW);
      server.send(200, "text/plain", "Light ON");
    }
  );

  server.on(
    "/light/off",
    []()
    {
      lightState = false;
      digitalWrite(LIGHT_RELAY, HIGH);
      server.send(200, "text/plain", "Light OFF");
    }
  );

  server.begin();

  Serial.println("Web Server Started");
}

// ================= LOOP =================

void loop()
{
  server.handleClient();

  if (!mqttClient.connected())
  {
    reconnectMQTT();
  }

  mqttClient.loop();

  // Read sensors every 5 seconds
  if (millis() - lastSensorRead >= 5000)
  {
    readSensors();

    applyAutomationRules();

    updateOLED();

    lastSensorRead = millis();
  }

  // MQTT every 30 seconds
  if (millis() - lastMQTTPublish >= 30000)
  {
    publishMQTT();

    lastMQTTPublish = millis();
  }
}

// ================= WIFI =================

void connectWiFi()
{
  Serial.print("Connecting WiFi");

  WiFi.begin(
    ssid,
    password
  );

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);

    Serial.print(".");
  }

  Serial.println();

  Serial.println("WiFi Connected");

  Serial.print("IP Address: ");

  Serial.println(
    WiFi.localIP()
  );
}

// ================= READ SENSORS =================

void readSensors()
{
  // DHT11
  float newTemperature = dht.readTemperature();

  float newHumidity = dht.readHumidity();

  if (!isnan(newTemperature))
  {
    temperature = newTemperature;
  }

  if (!isnan(newHumidity))
  {
    humidity = newHumidity;
  }

  // MQ2
  gasValue = analogRead(MQ2_PIN);

  gasPercent = map(
    gasValue,
    0,
    4095,
    0,
    100
  );

  gasPercent = constrain(
    gasPercent,
    0,
    100
  );

  // PIR
  pirState = digitalRead(PIR_PIN);

  // LDR
  ldrValue = analogRead(LDR_PIN);

  lightPercent = map(
    ldrValue,
    0,
    4095,
    0,
    100
  );

  lightPercent = constrain(
    lightPercent,
    0,
    100
  );

  if (pirState == HIGH)
  {
    lastMotionTime = millis();
  }

  Serial.println("--------------------");

  Serial.print("Temperature: ");
  Serial.println(temperature);

  Serial.print("Humidity: ");
  Serial.println(humidity);

  Serial.print("Gas: ");
  Serial.println(gasPercent);

  Serial.print("PIR: ");
  Serial.println(pirState);

  Serial.print("Light: ");
  Serial.println(lightPercent);
}

// ================= AUTOMATION =================

void applyAutomationRules()
{
  // ================= GAS SAFETY =================

  if (gasPercent > 60)
  {
    gasAlert = true;

    // Safety shutdown
    fanState = false;
    lightState = false;

    digitalWrite(
      FAN_RELAY,
      HIGH
    );

    digitalWrite(
      LIGHT_RELAY,
      HIGH
    );

    digitalWrite(
      BUZZER_PIN,
      HIGH
    );

    digitalWrite(
      RED_LED,
      HIGH
    );

    digitalWrite(
      GREEN_LED,
      LOW
    );

    return;
  }

  else
  {
    gasAlert = false;

    digitalWrite(
      BUZZER_PIN,
      LOW
    );

    digitalWrite(
      RED_LED,
      LOW
    );

    digitalWrite(
      GREEN_LED,
      HIGH
    );
  }

  // ================= FAN AUTOMATION =================

  // Temperature above 32°C
  if (!fanState && temperature > 32)
  {
    fanState = true;

    digitalWrite(
      FAN_RELAY,
      LOW
    );

    Serial.println("Fan AUTO ON");
  }

  // Temperature below 28°C
  if (fanState && temperature < 28)
  {
    fanState = false;

    digitalWrite(
      FAN_RELAY,
      HIGH
    );

    Serial.println("Fan AUTO OFF");
  }

  // ================= LIGHT AUTOMATION =================

  // Dark + Motion
  if (
    lightPercent < 20 &&
    pirState == HIGH
  )
  {
    lightState = true;

    digitalWrite(
      LIGHT_RELAY,
      LOW
    );

    Serial.println("Light AUTO ON");
  }

  // Daylight
  if (lightPercent > 60)
  {
    lightState = false;

    digitalWrite(
      LIGHT_RELAY,
      HIGH
    );

    Serial.println("Light OFF - Daylight");
  }

  // No motion for 3 minutes
  if (
    millis() - lastMotionTime >= 180000
  )
  {
    lightState = false;

    digitalWrite(
      LIGHT_RELAY,
      HIGH
    );

    Serial.println(
      "Light OFF - No Motion"
    );
  }
}

// ================= MQTT =================

void reconnectMQTT()
{
  while (!mqttClient.connected())
  {
    Serial.print("Connecting MQTT...");

    String clientID = "ESP32HomeHub";

    clientID += String(
      random(0xffff),
      HEX
    );

    if (
      mqttClient.connect(
        clientID.c_str()
      )
    )
    {
      Serial.println(
        "MQTT Connected"
      );
    }

    else
    {
      Serial.print(
        "Failed, rc="
      );

      Serial.println(
        mqttClient.state()
      );

      delay(2000);
    }
  }
}

// ================= MQTT PUBLISH =================

void publishMQTT()
{
  String json = "{";

  json += "\"temp\":";
  json += String(temperature, 1);

  json += ",\"humidity\":";
  json += String(humidity, 1);

  json += ",\"gas\":";
  json += String(gasPercent);

  json += ",\"pir\":";
  json += String(pirState);

  json += ",\"light\":";
  json += String(lightPercent);

  json += ",\"fan\":";
  json += String(fanState ? 1 : 0);

  json += ",\"light_relay\":";
  json += String(lightState ? 1 : 0);

  json += ",\"alert\":";
  json += String(gasAlert ? 1 : 0);

  json += "}";

  mqttClient.publish(
    mqttTopic.c_str(),
    json.c_str()
  );

  Serial.println(
    "MQTT Published:"
  );

  Serial.println(json);
}

// ================= OLED =================

void updateOLED()
{
  if (
    millis() - lastOLEDUpdate >= 5000
  )
  {
    oledScreen++;

    if (oledScreen > 2)
    {
      oledScreen = 0;
    }

    lastOLEDUpdate = millis();
  }

  display.clearDisplay();

  display.setCursor(0, 0);

  // Screen 1
  if (oledScreen == 0)
  {
    display.println(
      "ENVIRONMENT"
    );

    display.println();

    display.print(
      "Temp: "
    );

    display.print(
      temperature
    );

    display.println(
      " C"
    );

    display.print(
      "Humidity: "
    );

    display.print(
      humidity
    );

    display.println(
      " %"
    );
  }

  // Screen 2
  else if (oledScreen == 1)
  {
    display.println(
      "SENSORS"
    );

    display.println();

    display.print(
      "Gas: "
    );

    display.print(
      gasPercent
    );

    display.println(
      " %"
    );

    display.print(
      "PIR: "
    );

    display.println(
      pirState ? "MOTION" : "NO MOTION"
    );

    display.print(
      "LDR: "
    );

    display.print(
      lightPercent
    );

    display.println(
      " %"
    );
  }

  // Screen 3
  else
  {
    display.println(
      "SYSTEM STATUS"
    );

    display.println();

    display.print(
      "Fan: "
    );

    display.println(
      fanState ? "ON" : "OFF"
    );

    display.print(
      "Light: "
    );

    display.println(
      lightState ? "ON" : "OFF"
    );

    display.print(
      "Alert: "
    );

    display.println(
      gasAlert ? "YES" : "NO"
    );
  }

  display.display();
}

// ================= WEB DASHBOARD =================
// NOTE: Only the visual design (HTML/CSS) below has been
// redesigned to look more attractive and user-friendly.
// All variables, values, and routes used are EXACTLY the
// same as before - no automation/sensor/MQTT logic changed.

void handleRoot()
{
  String html = "";

  html += "<!DOCTYPE html>";
  html += "<html lang='en'>";
  html += "<head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<meta http-equiv='refresh' content='5'>";
  html += "<title>ESP32 IoT Home Hub</title>";
  html += "<style>";

  html += "*{box-sizing:border-box;margin:0;padding:0;}";

  html += "body{";
  html += "font-family:'Segoe UI',Roboto,Arial,sans-serif;";
  html += "background:linear-gradient(135deg,#1f2937,#0f172a);";
  html += "min-height:100vh;";
  html += "padding:24px 14px 60px;";
  html += "color:#e5e7eb;";
  html += "}";

  html += ".wrap{max-width:760px;margin:0 auto;}";

  html += "header{text-align:center;margin-bottom:24px;}";

  html += "header h1{";
  html += "font-size:1.6rem;";
  html += "font-weight:700;";
  html += "background:linear-gradient(90deg,#38bdf8,#818cf8);";
  html += "-webkit-background-clip:text;";
  html += "background-clip:text;";
  html += "color:transparent;";
  html += "}";

  html += "header p{color:#94a3b8;font-size:0.85rem;margin-top:4px;}";

  html += ".alert-banner{";
  html += "text-align:center;";
  html += "padding:12px;";
  html += "border-radius:12px;";
  html += "font-weight:700;";
  html += "letter-spacing:0.5px;";
  html += "margin-bottom:20px;";
  html += "font-size:1rem;";
  html += "}";

  html += ".alert-safe{background:rgba(34,197,94,0.15);color:#4ade80;border:1px solid rgba(34,197,94,0.4);}";
  html += ".alert-danger{background:rgba(239,68,68,0.18);color:#f87171;border:1px solid rgba(239,68,68,0.5);";
  html += "animation:pulse 1.2s infinite;}";

  html += "@keyframes pulse{0%{opacity:1;}50%{opacity:0.55;}100%{opacity:1;}}";

  html += ".grid{";
  html += "display:grid;";
  html += "grid-template-columns:repeat(auto-fit,minmax(220px,1fr));";
  html += "gap:16px;";
  html += "margin-bottom:16px;";
  html += "}";

  html += ".card{";
  html += "background:rgba(255,255,255,0.06);";
  html += "border:1px solid rgba(255,255,255,0.08);";
  html += "backdrop-filter:blur(6px);";
  html += "border-radius:16px;";
  html += "padding:18px 20px;";
  html += "box-shadow:0 4px 14px rgba(0,0,0,0.25);";
  html += "}";

  html += ".card h2{";
  html += "font-size:0.85rem;";
  html += "text-transform:uppercase;";
  html += "letter-spacing:1px;";
  html += "color:#93c5fd;";
  html += "margin-bottom:12px;";
  html += "display:flex;align-items:center;gap:8px;";
  html += "}";

  html += ".row{";
  html += "display:flex;";
  html += "justify-content:space-between;";
  html += "align-items:center;";
  html += "padding:6px 0;";
  html += "border-bottom:1px dashed rgba(255,255,255,0.08);";
  html += "}";

  html += ".row:last-child{border-bottom:none;}";

  html += ".row .label{color:#cbd5e1;font-size:0.9rem;}";
  html += ".row .value{font-weight:700;font-size:1.05rem;}";

  html += ".pill{";
  html += "display:inline-block;";
  html += "padding:3px 12px;";
  html += "border-radius:999px;";
  html += "font-size:0.8rem;";
  html += "font-weight:700;";
  html += "}";

  html += ".pill-on{background:rgba(34,197,94,0.18);color:#4ade80;}";
  html += ".pill-off{background:rgba(148,163,184,0.18);color:#94a3b8;}";
  html += ".pill-motion{background:rgba(56,189,248,0.18);color:#38bdf8;}";
  html += ".pill-nomotion{background:rgba(148,163,184,0.18);color:#94a3b8;}";

  html += ".btn-row{display:flex;gap:10px;margin-top:14px;}";

  html += ".btn{";
  html += "flex:1;";
  html += "text-align:center;";
  html += "padding:10px 8px;";
  html += "border-radius:10px;";
  html += "text-decoration:none;";
  html += "font-weight:600;";
  html += "font-size:0.85rem;";
  html += "transition:transform 0.15s ease;";
  html += "}";

  html += ".btn:active{transform:scale(0.96);}";

  html += ".btn-on{background:#22c55e;color:#052e14;}";
  html += ".btn-off{background:#334155;color:#e2e8f0;}";

  html += "footer{";
  html += "text-align:center;";
  html += "color:#64748b;";
  html += "font-size:0.75rem;";
  html += "margin-top:24px;";
  html += "}";

  html += "</style>";
  html += "</head>";
  html += "<body>";
  html += "<div class='wrap'>";

  html += "<header>";
  html += "<h1>&#127968; ESP32 IoT Home Automation Hub</h1>";
  html += "<p>Live sensor data &bull; auto-refreshes every 5 seconds</p>";
  html += "</header>";

  if (gasAlert)
  {
    html += "<div class='alert-banner alert-danger'>&#9888; GAS ALERT - SAFETY SHUTDOWN ACTIVE</div>";
  }
  else
  {
    html += "<div class='alert-banner alert-safe'>&#10003; ALL SYSTEMS SAFE</div>";
  }

  html += "<div class='grid'>";

  // Environment card
  html += "<div class='card'>";
  html += "<h2>&#127777; Environment</h2>";

  html += "<div class='row'><span class='label'>Temperature</span>";
  html += "<span class='value'>";
  html += String(temperature);
  html += " &deg;C</span></div>";

  html += "<div class='row'><span class='label'>Humidity</span>";
  html += "<span class='value'>";
  html += String(humidity);
  html += " %</span></div>";

  html += "</div>";

  // Sensors card
  html += "<div class='card'>";
  html += "<h2>&#128269; Sensors</h2>";

  html += "<div class='row'><span class='label'>Gas Level</span>";
  html += "<span class='value'>";
  html += String(gasPercent);
  html += " %</span></div>";

  html += "<div class='row'><span class='label'>Motion (PIR)</span>";
  if (pirState)
  {
    html += "<span class='pill pill-motion'>MOTION</span>";
  }
  else
  {
    html += "<span class='pill pill-nomotion'>NO MOTION</span>";
  }
  html += "</div>";

  html += "<div class='row'><span class='label'>Light Level</span>";
  html += "<span class='value'>";
  html += String(lightPercent);
  html += " %</span></div>";

  html += "</div>";

  html += "</div>"; // end grid

  html += "<div class='grid'>";

  // Fan control card
  html += "<div class='card'>";
  html += "<h2>&#128168; Fan Relay</h2>";

  html += "<div class='row'><span class='label'>Status</span>";
  if (fanState)
  {
    html += "<span class='pill pill-on'>ON</span>";
  }
  else
  {
    html += "<span class='pill pill-off'>OFF</span>";
  }
  html += "</div>";

  html += "<div class='btn-row'>";
  html += "<a class='btn btn-on' href='/fan/on'>Turn ON</a>";
  html += "<a class='btn btn-off' href='/fan/off'>Turn OFF</a>";
  html += "</div>";

  html += "</div>";

  // Light control card
  html += "<div class='card'>";
  html += "<h2>&#128161; Light Relay</h2>";

  html += "<div class='row'><span class='label'>Status</span>";
  if (lightState)
  {
    html += "<span class='pill pill-on'>ON</span>";
  }
  else
  {
    html += "<span class='pill pill-off'>OFF</span>";
  }
  html += "</div>";

  html += "<div class='btn-row'>";
  html += "<a class='btn btn-on' href='/light/on'>Turn ON</a>";
  html += "<a class='btn btn-off' href='/light/off'>Turn OFF</a>";
  html += "</div>";

  html += "</div>";

  html += "</div>"; // end grid

  html += "<footer>ESP32 Home Automation Hub &bull; IIT Jammu Summer School 2026</footer>";

  html += "</div>"; // end wrap
  html += "</body>";
  html += "</html>";

  server.send(
    200,
    "text/html",
    html
  );
}

/***************************************************
 *  AI Enabled Plant Watering System
 *  ESP8266 + Blynk + Weather API + Cloud ML
 *  Serial Only (No LCD)
 ***************************************************/

// ================== BLYNK ==================
#define BLYNK_TEMPLATE_ID "template_id"
#define BLYNK_TEMPLATE_NAME "Plant Watering System"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_TOKEN"

// ================== LIBRARIES ==================
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

// ================== WIFI ==================
char ssid[] = "YOUR_WIFI";
char pass[] = "YOUR_PASSWORD";

// ================== WEATHER API ==================
String apiKey = "YOUR_API_KEY";
String city   = "city,IN";

// ================== OBJECTS ==================

BlynkTimer timer;
bool manualMode = false;
bool bannerPrinted = false;

// 👇 ADD THESE TWO LINES
unsigned long lastManualTime = 0;
const unsigned long MANUAL_COOLDOWN = 15000; // 15 seconds



// ================== PINS ==================
#define SENSOR_PIN A0
#define PUMP_PIN   D3   // LOW = ON

// ================== CALIBRATION ==================
#define DRY_VALUE  900
#define WET_VALUE  300

// ================== GLOBAL WEATHER ==================
float temperature = 0;
int humidity = 0;
int rain = 0;
int lightIntensity = 0;   // Virtual LDR (0–100%)


// ================== FUNCTION DECLARATIONS ==================
void readSoilMoisture();
void getWeatherData();
void sendToAI(int moisture);

// ================== SETUP ==================
void setup() {
  Serial.begin(115200);
  delay(2000);   // 👈 IMPORTANT

  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, HIGH);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  Serial.println("\n===============================");
  Serial.println("🌱 AI Plant Watering System");
  Serial.println("===============================");

  getWeatherData();   // weather AFTER connection

  timer.setInterval(3000L, readSoilMoisture);
timer.setInterval(60000L, getWeatherData); // every 1 minute
}


// ================== BLYNK MANUAL CONTROL ==================
BLYNK_WRITE(V1) {
  manualMode = param.asInt();
  lastManualTime = millis();   // 👈 record manual action time

  if (manualMode) {
    digitalWrite(PUMP_PIN, LOW);
    Serial.println("🧑 Manual Mode → Pump ON");
  } else {
    digitalWrite(PUMP_PIN, HIGH);
    Serial.println("🧑 Manual Mode → Pump OFF");
  }
}


// ================== READ SOIL SENSOR ==================
void readSoilMoisture() {
  int raw = analogRead(SENSOR_PIN);

  int moisture = map(raw, DRY_VALUE, WET_VALUE, 0, 100);
  moisture = constrain(moisture, 0, 100);

  Blynk.virtualWrite(V0, moisture);

  Serial.print("🌾 Raw ADC: ");
  Serial.print(raw);
  Serial.print(" | Moisture: ");
  Serial.print(moisture);
  Serial.println("%");

 if (!manualMode && (millis() - lastManualTime > MANUAL_COOLDOWN)) {
  sendToAI(moisture);
}

}

// ================== WEATHER API FUNCTION ==================
void getWeatherData() {

  if (WiFi.status() != WL_CONNECTED) return;

  WiFiClient client;
  HTTPClient http;

 String url = "write url"

  http.begin(client, url);
  int httpCode = http.GET();

  if (httpCode == 200) {

    String payload = http.getString();
    DynamicJsonDocument doc(2048);
    deserializeJson(doc, payload);

    temperature = doc["main"]["temp"];
    humidity = doc["main"]["humidity"];
    String weatherMain = doc["weather"][0]["main"];
    rain = (weatherMain == "Rain") ? 1 : 0;
    int cloudCover = doc["clouds"]["all"];   // 0–100 %
lightIntensity = map(cloudCover, 100, 0, 0, 100);
lightIntensity = constrain(lightIntensity, 0, 100);


    Serial.println("🌦 Weather Updated");
    Serial.print("Temp: "); Serial.print(temperature); Serial.println(" °C");
    Serial.print("Humidity: "); Serial.print(humidity); Serial.println(" %");
    Serial.print("Rain: "); Serial.println(rain ? "YES" : "NO");
    Serial.print("Light Intensity (Virtual LDR): ");
Serial.print(lightIntensity);
Serial.println(" %");


  } else {
    Serial.println("❌ Weather API Error");
  }

  http.end();
}

// ================== AI / ML FUNCTION ==================
void sendToAI(int moisture) {

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("❌ WiFi Disconnected");
    return;
  }

  WiFiClient client;
  HTTPClient http;

  http.begin(client, "http://192.168.43.134:5000/predict");
  http.addHeader("Content-Type", "application/json");

  String payload = "{";
payload += "\"moisture\":" + String(moisture) + ",";
payload += "\"temp\":" + String(temperature) + ",";
payload += "\"humidity\":" + String(humidity) + ",";
payload += "\"rain\":" + String(rain) + ",";
payload += "\"light\":" + String(lightIntensity);
payload += "}";


  int httpCode = http.POST(payload);

  if (httpCode == 200) {

    String response = http.getString();
    StaticJsonDocument<200> doc;

    if (deserializeJson(doc, response) == DeserializationError::Ok) {

      int pump = doc["pump"];

      if (pump == 1) {
        digitalWrite(PUMP_PIN, LOW);
        Serial.println("🤖 AI Decision → Pump ON");
      } else {
        digitalWrite(PUMP_PIN, HIGH);
        Serial.println("🤖 AI Decision → Pump OFF");
      }

    } else {
      Serial.println("❌ JSON Parse Error");
    }

  } else {
    Serial.println("❌ AI Server Down → Pump OFF");
    digitalWrite(PUMP_PIN, HIGH);
  }

  http.end();
}

// ================== LOOP ==================
void loop() {
  Blynk.run();
  timer.run();

  if (!bannerPrinted) {
    Serial.println("\n===============================");
    Serial.println("🌱 AI Plant Watering System Running...");
    Serial.println("===============================");
    bannerPrinted = true;
  }
}


// ============================================
// sensor_hub.ino — Arduino Nano
// Reads MQ6, MQ2, MQ135, Sharp Dust Sensor, DHT11
// Displays on 20x4 LCD
// Sends CSV string via Serial to ESP8266
// ============================================

#include <LiquidCrystal.h>
#include "DHT.h"

// --- LCD Setup ---
const int rs = 3, en = 4, d4 = 5, d5 = 6, d6 = 7, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// --- DHT11 Setup ---
#define DHTPIN 9
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// --- Gas Sensor Pins ---
const int DOUTpin1 = 11;  // MQ6 digital out
const int DOUTpin2 = 12;  // MQ2 digital out
const int DOUTpin3 = 13;  // MQ135 digital out
int limit1, value1;
int limit2, value2;
int limit3, value3;

// --- Sharp Dust Sensor ---
int measurePin = A2;
int ledPower   = 2;
unsigned int samplingTime = 280;
unsigned int deltaTime    = 40;
unsigned int sleepTime    = 9680;
float voMeasured  = 0;
float calcVoltage = 0;
float dustDensity = 0;

String readstringdata = "";

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(ledPower, OUTPUT);
  lcd.begin(20, 4);
  lcd.setCursor(0, 0);
  lcd.print(" Emissions Tracker ");
  delay(200);
}

void loop() {
  readstringdata = "";

  // --- MQ6 (LPG/Gas) ---
  value1 = analogRead(A3);
  limit1 = digitalRead(DOUTpin1);

  // --- MQ2 (Smoke) ---
  value2 = analogRead(A1);
  limit2 = digitalRead(DOUTpin2);

  // --- MQ135 (Air Quality) ---
  value3 = analogRead(A0);
  limit3 = digitalRead(DOUTpin3);

  lcd.setCursor(0, 1);
  lcd.print("MQ6:");
  lcd.print(value1);
  lcd.print("  MQ2:");
  lcd.print(value2);
  lcd.print(" ");

  // --- Sharp Dust Sensor ---
  digitalWrite(ledPower, LOW);
  delayMicroseconds(samplingTime);
  voMeasured = analogRead(measurePin);
  delayMicroseconds(deltaTime);
  digitalWrite(ledPower, HIGH);
  delayMicroseconds(sleepTime);

  calcVoltage = voMeasured * (5.0 / 1024);
  dustDensity = 0.17 * calcVoltage - 0.1;

  lcd.setCursor(0, 2);
  lcd.print("MQ135:");
  lcd.print(value3);
  lcd.print(" Dust:");
  lcd.print(dustDensity);
  lcd.print(" ");
  delay(500);

  // --- DHT11 (Temperature + Humidity) ---
  float h = dht.readHumidity();
  float t = dht.readTemperature();        // Celsius
  float f = dht.readTemperature(true);    // Fahrenheit

  if (isnan(h) || isnan(t) || isnan(f)) {
    return;  // Retry on next loop if sensor read fails
  }

  float hic = dht.computeHeatIndex(t, h, false);

  lcd.setCursor(0, 3);
  lcd.print("T:");
  lcd.print(t, 0);
  lcd.print("degC ");
  lcd.print(" Hum:");
  lcd.print(h, 0);
  lcd.print("%");
  lcd.print("  ");

  // --- Build CSV string for ESP8266 ---
  // Format: humidity,temperature,mq6,mq2,mq135,dustDensity#
  readstringdata += String(h);
  readstringdata += ",";
  readstringdata += String(t);
  readstringdata += ",";
  readstringdata += String(value1);   // MQ6
  readstringdata += ",";
  readstringdata += String(value2);   // MQ2
  readstringdata += ",";
  readstringdata += String(value3);   // MQ135
  readstringdata += ",";
  readstringdata += String(dustDensity);
  readstringdata += "#";              // End delimiter

  Serial.println(readstringdata);

  readstringdata = "";
  delay(5000);
}

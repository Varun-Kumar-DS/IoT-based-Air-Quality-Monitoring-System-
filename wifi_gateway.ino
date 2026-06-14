#include <Arduino.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <WiFiClient.h>
#include <ThingSpeak.h>

// ⚠️ Copy config.example.h → config.h and fill in your credentials
#include "config.h"

WiFiClient client;

String readstring = "";
String h, t;
String mq6, mq2, mq135;
String dust;
int ind1, ind2, ind3, ind4, ind5, ind6;

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.print("Connecting to AP");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  ThingSpeak.begin(client);
}

void loop() {
  readstring = "";

  while (Serial.available()) {
    delay(10);
    char c = Serial.read();
    if (c == '#') { break; }
    readstring += c;
  }

  if (readstring.length() > 0) {
    Serial.println(readstring);

    ind1 = readstring.indexOf(',');
    h    = readstring.substring(0, ind1);

    ind2 = readstring.indexOf(',', ind1 + 1);
    t    = readstring.substring(ind1 + 1, ind2);

    ind3 = readstring.indexOf(',', ind2 + 1);
    mq6  = readstring.substring(ind2 + 1, ind3);

    ind4 = readstring.indexOf(',', ind3 + 1);
    mq2  = readstring.substring(ind3 + 1, ind4);

    ind5 = readstring.indexOf(',', ind4 + 1);
    mq135 = readstring.substring(ind4 + 1, ind5);

    ind6 = readstring.indexOf(',', ind5 + 1);
    dust = readstring.substring(ind5 + 1);

    Serial.print("Humidity: ");     Serial.println(h);
    Serial.print("Temperature: ");  Serial.println(t);
    Serial.print("MQ6: ");          Serial.println(mq6);
    Serial.print("MQ2: ");          Serial.println(mq2);
    Serial.print("MQ135: ");        Serial.println(mq135);
    Serial.print("Dust Density: "); Serial.println(dust);

    ThingSpeak.setField(1, h);
    ThingSpeak.setField(2, t);
    ThingSpeak.setField(3, mq6);
    ThingSpeak.setField(4, mq2);
    ThingSpeak.setField(5, mq135);
    ThingSpeak.setField(6, dust);

    ThingSpeak.writeFields(THINGSPEAK_CHANNEL, THINGSPEAK_API_KEY);

    delay(1000);
  }
}

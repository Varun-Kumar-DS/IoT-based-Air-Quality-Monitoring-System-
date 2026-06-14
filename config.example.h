// ============================================
// config.h — WiFi & ThingSpeak credentials
// ============================================
// 1. Copy this file:  config.example.h → config.h
// 2. Fill in your values below
// 3. config.h is in .gitignore — it will NOT be committed
// ============================================

#define WIFI_SSID         "YOUR_WIFI_SSID"
#define WIFI_PASSWORD      "YOUR_WIFI_PASSWORD"

unsigned long THINGSPEAK_CHANNEL = 0000000;              // Your ThingSpeak channel number
const char *  THINGSPEAK_API_KEY = "YOUR_WRITE_API_KEY";  // Your ThingSpeak Write API key

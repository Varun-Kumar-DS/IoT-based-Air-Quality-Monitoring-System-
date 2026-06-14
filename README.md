# 🌫️ IoT Air Quality Monitoring System using Machine Learning

> A real-time air quality monitoring system combining **embedded sensors**, **cloud data logging**, and **ML-based clustering analysis** — presented at the **National Conference 2024**.

[![Arduino](https://img.shields.io/badge/Arduino-00979D?style=flat&logo=arduino&logoColor=white)]()
[![Python](https://img.shields.io/badge/Python-3.10+-3776AB?logo=python&logoColor=white)]()
[![scikit-learn](https://img.shields.io/badge/scikit--learn-F7931E?logo=scikit-learn&logoColor=white)]()
[![ThingSpeak](https://img.shields.io/badge/ThingSpeak-Cloud-0078D4)]()
[![Conference](https://img.shields.io/badge/📄_National_Conference-2024-e11d48?style=flat)]()

---

## 📌 Overview

Indoor and outdoor air pollution is one of the leading causes of preventable death globally, yet real-time monitoring remains expensive and inaccessible. This project builds a **low-cost, IoT-based air quality monitoring system** that:

1. **Senses** — reads gas concentration (MQ6, MQ2, MQ135), particulate dust density (Sharp GP2Y1010AU0F), temperature, and humidity (DHT11) via an Arduino Nano
2. **Transmits** — pushes live readings to **ThingSpeak** cloud via an ESP8266 NodeMCU over WiFi
3. **Analyses** — applies **Agglomerative (Hierarchical) Clustering** on collected sensor data to classify air quality levels
4. **Displays** — shows real-time readings on a 20x4 LCD and via a companion Android app

## 🖼️ Hardware

<p align="center">
  <img src="images/hardware-setup.jpg" width="400" alt="IoT Air Quality Monitor hardware setup" />
</p>

The board integrates an **Arduino Nano** (sensor acquisition + LCD display) and an **ESP8266 NodeMCU** (WiFi + cloud upload) on a custom perfboard, with 5 sensors reading environmental data in real time.

## 🏗️ System Architecture

```
┌─────────────────────────────────────────────────────────┐
│                    SENSOR LAYER                         │
│  MQ6 (LPG/Gas) · MQ2 (Smoke) · MQ135 (Air Quality)    │
│  Sharp Dust Sensor · DHT11 (Temp + Humidity)            │
└──────────────────────┬──────────────────────────────────┘
                       │ Analog/Digital
                       ▼
┌──────────────────────────────────────────────────────────┐
│              ARDUINO NANO (Sensor Hub)                   │
│  Reads all sensors → formats data → LCD display          │
│  Sends CSV string via Serial to ESP8266                  │
└──────────────────────┬───────────────────────────────────┘
                       │ Serial (9600 baud)
                       ▼
┌──────────────────────────────────────────────────────────┐
│              ESP8266 NodeMCU (WiFi Gateway)               │
│  Parses serial data → pushes to ThingSpeak via HTTP       │
└──────────────────────┬───────────────────────────────────┘
                       │ WiFi / HTTP
                       ▼
┌──────────────────────────────────────────────────────────┐
│              THINGSPEAK CLOUD                            │
│  Stores time-series data · REST API · Visualisations     │
└──────────────────────┬───────────────────────────────────┘
                       │
              ┌────────┴────────┐
              ▼                 ▼
┌──────────────────┐  ┌──────────────────────┐
│   Android App    │  │  Python ML Analysis  │
│  (Real-time AQI) │  │  (Clustering)        │
└──────────────────┘  └──────────────────────┘
```

## 🔬 ML Analysis — Agglomerative Clustering

The collected sensor data was analysed offline using **Hierarchical (Agglomerative) Clustering** to classify air quality readings into distinct pollution levels.

**Pipeline:**
1. Normalised sensor readings (MQ6, MQ135, dust density) using sklearn's `normalize()`
2. Generated dendrograms using Ward's linkage to determine optimal cluster count
3. Applied Agglomerative Clustering with `n_clusters=2` (clean air vs polluted air)
4. Visualised cluster assignments per sensor type (MQ6, MQ135, dust density)

**Key finding:** MQ135 and dust density readings showed the clearest cluster separation, making them the most reliable indicators for binary air quality classification in this sensor configuration.

## 🛠️ Tech Stack

| Layer | Technology |
|---|---|
| **Microcontrollers** | Arduino Nano, ESP8266 NodeMCU |
| **Sensors** | MQ6 (LPG), MQ2 (Smoke), MQ135 (Air Quality), Sharp GP2Y1010AU0F (Dust), DHT11 (Temp/Humidity) |
| **Display** | 20x4 LCD (LiquidCrystal) |
| **Cloud** | ThingSpeak (IoT platform) |
| **ML** | Python, scikit-learn (Agglomerative Clustering), SciPy, Matplotlib |
| **Mobile** | Android companion app |

## 📁 Structure

```
├── hardware/
│   ├── sensor_hub.ino              # Arduino Nano — sensor reading + LCD
│   └── wifi_gateway.ino            # ESP8266 — WiFi + ThingSpeak upload
├── analysis/
│   └── clustering.py               # Agglomerative Clustering on sensor data
├── data/
│   ├── airquality_dataset.csv      # Collected sensor readings (209 samples)
│   └── feeds.csv                   # ThingSpeak export (1328 time-series entries)
├── images/
│   └── hardware-setup.jpg          # Photo of the assembled hardware
├── config.example.h                # Template for WiFi + ThingSpeak credentials
├── requirements.txt
└── README.md
```

## 🚀 Quickstart

### Hardware
1. Flash `hardware/sensor_hub.ino` to the Arduino Nano
2. Copy `config.example.h` → `config.h` and fill in your WiFi SSID/password and ThingSpeak API key
3. Flash `hardware/wifi_gateway.ino` to the ESP8266 NodeMCU
4. Connect Arduino TX → ESP8266 RX (Serial at 9600 baud)

### ML Analysis
```bash
cd analysis/
pip install -r ../requirements.txt
python clustering.py
```

## ⚠️ Credentials

This project connects to WiFi and ThingSpeak. **Never commit real credentials.**

1. Copy `config.example.h` to `config.h`
2. Fill in your own values
3. `config.h` is already in `.gitignore`

## 📚 What I Learned

- **Hardware-software integration is where most IoT projects fail** — serial communication between Arduino and ESP8266 required careful string formatting (CSV with `#` delimiter) and baud rate matching. A mismatch at any point meant silent data loss with no error message
- **Sensor calibration matters more than model selection** — MQ-series sensors are noisy and influenced by temperature/humidity. Raw analog values aren't directly comparable across sessions without baseline calibration
- **ThingSpeak is great for prototyping, limiting for production** — the free tier's 15-second update interval was sufficient for this project but would be a bottleneck for real-time alerting. A production system would need MQTT or a custom backend
- **Hierarchical clustering was the right choice here** — with a small dataset (209 samples) from known sensor distributions, dendrograms gave interpretable cluster boundaries. K-Means would have worked but offered less insight into the data's hierarchical structure

## 📄 Conference

This project was presented at the **National Conference 2024**. The work demonstrated a viable low-cost alternative to commercial AQI monitoring stations using off-the-shelf components totalling under ₹2,000.

## 📬 Contact

**Varun Kumar** · [Portfolio](https://varun-kumar-ds.github.io) · [LinkedIn](https://www.linkedin.com/in/varun-kumar-ai) · [varunzayne@gmail.com](mailto:varunzayne@gmail.com)

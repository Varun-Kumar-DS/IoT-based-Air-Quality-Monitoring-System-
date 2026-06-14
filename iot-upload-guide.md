# Upload Guide — IoT Air Quality Monitoring Repo

## ⚠️ FIRST: Change your compromised credentials

You shared these publicly in this chat. Change them NOW before uploading anything:

1. **ThingSpeak** → go to thingspeak.com → Account → change password → then go to your channel → API Keys → regenerate the Write API key
2. **WiFi router** → change the password on your TP-Link router admin panel

---

## 📦 Repo name: `iot-air-quality-monitoring`

### What goes where

```
iot-air-quality-monitoring/
├── hardware/
│   ├── sensor_hub.ino           ← use the cleaned file I gave you
│   └── wifi_gateway.ino         ← use the cleaned file I gave you (credentials removed)
├── analysis/
│   └── clustering.py            ← use clustering_clean.py (paths fixed)
├── data/
│   ├── airquality_dataset.csv   ← your uploaded file (5 KB — upload it)
│   └── feeds.csv                ← your uploaded file (56 KB — upload it)
├── images/
│   └── hardware-setup.jpg       ← your hardware photo
├── config.example.h             ← credential template (NO real values)
├── requirements.txt             ← use iot-requirements.txt
├── .gitignore                   ← use iot-gitignore.txt (rename to .gitignore)
└── README.md                    ← use iot-README.md
```

### What NOT to upload

| File | Why |
|---|---|
| `config.h` (with real credentials) | Security risk — .gitignore blocks it |
| `IoTAirQualityMonitoring.apk` | 3.5 MB binary, can't be code-reviewed, looks unprofessional |
| Any file with hardcoded passwords/API keys | Security risk |

---

## 🚀 Step-by-step upload

### Step 1 — Create the repo

1. Go to https://github.com/new
2. Name: `iot-air-quality-monitoring`
3. Description: "IoT-based air quality monitoring system with ML clustering — presented at National Conference 2024"
4. Public ✅
5. Add a README ✅
6. .gitignore: choose Python
7. Click Create repository

### Step 2 — Create the folder structure + upload files

GitHub's web interface doesn't let you create folders directly, but there's a trick: when you create a new file, type `folder/filename` and it auto-creates the folder.

**For each file below, go to: Add file → Create new file**

1. Type `hardware/sensor_hub.ino` → paste the sensor_hub.ino contents → commit
2. Type `hardware/wifi_gateway.ino` → paste the wifi_gateway.ino contents → commit
3. Type `analysis/clustering.py` → paste the clustering_clean.py contents → commit
4. Type `config.example.h` → paste the config.example.h contents → commit

**For data files and image, go to: Add file → Upload files**

5. Upload `airquality_dataset.csv` — but first, create the data folder:
   - Create a dummy file: `data/.gitkeep` (empty file) → commit
   - Then: Add file → Upload files → drag in `airquality_dataset.csv`
   - Hmm, actually GitHub uploads to root. Easier method below.

### Easier method for folders: use GitHub Desktop or git CLI

If the web interface folder trick feels clunky, use Terminal:

```bash
# 1. Clone your new empty repo
git clone https://github.com/Varun-Kumar-DS/iot-air-quality-monitoring.git
cd iot-air-quality-monitoring

# 2. Create folder structure
mkdir -p hardware analysis data images

# 3. Copy files into the right folders
# (move the files I gave you into these folders on your Mac)

# 4. Add, commit, push
git add .
git commit -m "Initial commit: hardware code, ML analysis, sensor data, and README"
git push origin main
```

### Step 3 — Replace the README

1. Click README.md → pencil icon
2. Delete everything → paste iot-README.md contents
3. Commit

### Step 4 — Add topics

Click ⚙️ next to About → add topics:
`iot` `arduino` `esp8266` `air-quality` `machine-learning` `python` `thingspeak` `clustering` `sensors`

---

## ✅ Final check

After uploading, verify:
- [ ] Hardware photo shows in the README
- [ ] No real passwords/API keys anywhere in the repo
- [ ] Both .ino files reference `config.h` (not hardcoded credentials)
- [ ] clustering.py uses relative path (`../data/airquality_dataset.csv`), not `C://Users//...`
- [ ] config.example.h has placeholder values only

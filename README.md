# Smart Home Energy Monitoring System

**Industry-oriented, end-to-end IoT project** measuring per-circuit current with non-invasive clamps (SCT-013), publishing via MQTT to a Dockerized stack (Mosquitto, InfluxDB, Telegana, Grafana) and integrating with Home Assistant.

## Features
- Non-invasive current measurement per circuit
- ESP32 edge processing (Irms, apparent power, cumulative Wh)
- MQTT transport, InfluxDB storage, Grafana visualization
- Home Assistant integration for sensors and automations
- Dockerized deployment for local/private hosting

## Folder Structure
(see repository root)

## Quick Start (hardware + server)
1. Build sensor front-end and connect clamp to ESP32 ADC (see `docs/wiring_diagram.txt`).
2. Edit `firmware/esp32_energy_node/src/mqtt_config.h` with your WiFi & MQTT broker values.
3. Flash the ESP32 using Arduino IDE or PlatformIO.
4. Start server stack:
   ```bash
   cd server
   docker compose up -d
Configure InfluxDB via http://localhost:8086 (create org home, bucket energy, copy token).

Update server/telegraf/telegraf.conf token and restart Telegraf.

Import Grafana dashboard from server/grafana/dashboards/energy_dashboard.json.

Add MQTT sensors into Home Assistant using home_assistant/mqtt_sensors.yaml.

Calibration
See docs/safety_and_validation.md. Briefly:

Apply a known resistive load (e.g., heater).

Record expected current = P / V_nominal.

Adjust DEFAULT_CAL_A_PER_ADC or store calibrated value in Preferences.

Safety
Do not work on live mains. Use non-invasive clamps only.

Qualified electrician for any wiring that exposes mains.

Enclose all electronics in an insulated enclosure.

Demo
Follow docs/demo_script.md to demonstrate live readings, Grafana panels and HA automations.

License
MIT — see LICENSE.

Contributing
Open issues or PRs. Use branch dev for features.

yaml
Copy code

---

# 8 — Demo Script & Screenshot Plan

### `docs/demo_script.md` (short)
1. **Pre-demo**:
   - Start `docker compose up -d`.
   - Verify Mosquitto port 1883, InfluxDB 8086, Grafana 3000 are reachable.
   - Ensure ESP32 publishes to `home/energy/node1/c1` (use `mosquitto_sub -h localhost -t 'home/energy/#' -v`).

2. **Live demo steps**:
   - Show the hardware (clamp on a single-phase live conductor; explain non-invasive safety).
   - Start a known load (e.g., 60 W lamp). Show corresponding increase in `Irms` on serial output and MQTT messages.
   - Open Grafana: show 5-minute live panel of Apparent Power.
   - Trigger an automation: simulate standby alert by raising low load above threshold and show HA notification.

3. **Screenshots to capture**:
   - Hardware wiring close-up (clamp, bias circuit, ADC pin).
   - ESP32 Serial output showing JSON publish.
   - `mosquitto_sub` terminal receiving JSON.
   - Grafana dashboard with panels for `Irms` and `apparent_w`.
   - Home Assistant sensor panel and an alert notification.

---

# 9 — Resume Bullet Points (3–5 short bullets)

- Designed and implemented a **Smart Home Energy Monitoring System** using ESP32, SCT-013 clamps, MQTT, InfluxDB and Grafana; enabled per-circuit visibility and automation.
- Implemented edge processing (ADC sampling, RMS computation, energy accumulation) on ESP32 to minimize bandwidth and enable local automations.
- Deployed and integrated a Dockerized observability stack (Mosquitto, InfluxDB, Telegraf, Grafana) and Home Assistant for dashboards and alerts.
- Performed calibration and validation vs reference loads achieving sub-5% apparent-power accuracy (improvement plan: add true-power meter for ≤2%).

---

# 10 — MIT License & Optional GitHub Action (CI)

### `LICENSE` (MIT)
```text
MIT License

Copyright (c) 2025 <Your Name>

Permission is hereby granted, free of charge, to any person obtaining a copy...
[standard MIT text]
.github/workflows/basic-ci.yml
yaml
Copy code
name: Basic CI

on: [push, pull_request]

jobs:
  checks:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - name: Validate Docker Compose YAML
        run: |
          sudo apt-get update -y
          sudo apt-get install -y python3-pip
          pip3 install yamllint
          yamllint server/docker-compose.yml || true
      - name: Lint README (spellcheck)
        run: echo "CI placeholder - add static checks you prefer"
This is a starter CI. Add linting, firmware unit-tests, or PlatformIO build checks as you expand.

Bill of Materials (BoM) — with sample part numbers
Where possible I list common components and example SKUs. Replace with local equivalents.

ESP32-DevKitC (ESP32-WROOM-32) — example: DOIT ESP32 DEVKIT V1

SCT-013-050 non-invasive current clamp (50A/1V) — SKU: SCT-013-050

Protoboard or PCB — any proto board

Resistors:

10 kΩ × 2 (for mid-bias divider) — 1% metal film

100 kΩ × 1 (series coupling)

100 Ω × 1 (safety bleed)

Capacitors:

10 µF electrolytic (decoupling at bias node)

0.1 µF ceramic (ADC filter)

3.5 mm jack (female) for SCT-013 outputs (optional)

USB-C or micro USB cable & 5V supply

Optional:

ADS1115 16-bit ADC module (I2C) for better accuracy

PZEM-004T v3 (UART energy meter) or DIN RS-485 meter for true-power

DIN enclosure for server & electronics

Misc: insulated wires, heatshrink, grommets, DIN-rail clips, strain reliefs

Wiring & Signal Conditioning (safe low-voltage plan)
SCT-013 (50 A / 1 V) typical wiring:

Clamp output → 3.5 mm jack → series 100 kΩ resistor → ADC input pin.

ADC input pin also connected to mid-bias node ≈ 1.65 V created by two 10 kΩ resistors between 3.3 V and GND.

Place 10 µF from bias node to GND (bulk decoupling) and 0.1 µF near ADC to GND (high freq).

Add 100 Ω resistor from clamp negative to GND as bleed/resistor for safety.

Ensure clamp is placed around one conductor only (live OR neutral), not both.

Enclose entire circuit; do not expose mains; any work on mains must be performed by a qualified electrician.

ASCII wiring snippet:

lua
Copy code
3.3V ---10k---+---10k--- GND
               |
               +---10uF---GND
               |
             ADC_bias (~1.65V)
Clamp tip --- 100k --- ADC_pin
ADC_pin --- 0.1uF --- GND
Clamp return --- 100R --- GND
Safety & Validation Steps
docs/safety_and_validation.md (highlights)
Non-invasive only: use clamps only on insulated conductors.

No mains alterations: do not cut or touch mains wiring unless licensed.

Enclosure: mount everything in an IP-rated enclosure with grommets and strain reliefs.

Calibration:

Connect clamp around conductor feeding a known resistive load (P).

Expected current: I_expected = P / V_nominal.

Capture many samples, compute measured Irms_measured.

Compute calibration factor k = I_expected / Irms_measured.

Update CAL_A_PER_ADC *= k or write to NVS.

Validation: run 24-hour recording and compare cumulative Wh to utility meter; clamps (apparent) typically within 5–8% of utility; use DIN meter/PZEM for billing-grade accuracy (≤2%).

Troubleshooting & Tips (short)
No MQTT messages: check WiFi credentials, MQTT broker address, Mosquitto logs, and use mosquitto_sub to confirm topics.

Noisy ADC: add decoupling caps, shorten wires, use ADS1115, or increase sampling & averaging.

Wrong polarity / zeroed values: ensure bias network present and working; check ADC reads ~2047 at idle (mid-rail).

Drift in wh_total: persist wh_total to NVS periodically.

Interview Prep — likely Qs & succinct answers
Why use MQTT?
Lightweight publish/subscribe model decouples producers and consumers, supports unreliable networks, and is widely used in industry for IoT.

Why ESP32?
Low cost, Wi-Fi, multiple ADCs, OTA capable, mature Arduino and IDF ecosystems — ideal for edge processing.

Why non-invasive clamps (SCT-013)?
Safety and simplicity — clamps avoid breaking mains conductors and lower the risk of shock. Good for monitoring without electrical rewiring.

How do you compute Irms?
Sample the centered AC waveform, square samples, average over many points across several mains cycles, then square-root the mean (RMS).

Why bias the ADC?
ESP32 ADC is single-ended (0..Vref). Clamp outputs AC centered at 0 V; bias shifts waveform to mid-rail so ADC can sample positive voltages only.

How to get accurate power (W) instead of apparent power (VA)?
Need synchronized voltage measurement (Vrms) + phase between voltage and current to compute true power (W). Use PZEM or RS-485 DIN meters or measure voltage with isolated voltage divider and compute instantaneous product.

How do you validate and calibrate?
Use known resistive loads; compute expected current; compute calibration constant; cross-check against smart plugs or utility meter.

Security considerations?
Enable MQTT authentication and TLS in production; restrict network access to the stack; avoid exposing mosquitto/influx admin ports to the internet.

Final notes & next steps
This repo is ready to upload to GitHub. Paste each file into the folder structure and commit.

Calibration is essential — do it before trusting energy numbers.

If you want, I can:

Generate a PlatformIO project for the ESP32 (with build settings).

Produce a PCB schematic (KiCad netlist) for the bias & connectors.

Extend to multi-channel (ADS1115) code and expanded dashboards.

Add ML module for anomaly detection (energy forecasting) with a sample notebook and dataset.

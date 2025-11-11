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

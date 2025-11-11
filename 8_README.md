# Smart Home Energy Monitoring System

An IoT-based system using ESP32 + SCT013 for real-time current monitoring. Data is streamed via MQTT to InfluxDB, visualized in Grafana, and automated with Home Assistant.

## Setup
1. Flash firmware to ESP32.
2. Run docker-compose for Mosquitto, InfluxDB, Grafana.
3. Import dashboards and view energy data.

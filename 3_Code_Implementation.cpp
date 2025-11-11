#include <WiFi.h>
#include <PubSubClient.h>
#include <Preferences.h>
#include <ArduinoJson.h>

const char* WIFI_SSID = "YOUR_SSID";
const char* WIFI_PASS = "YOUR_PASS";
const char* MQTT_HOST = "192.168.1.10";

WiFiClient espClient;
PubSubClient mqtt(espClient);

const int ADC_PIN = 34;
const float V_NOM = 230.0;
float CAL_A_PER_ADC = 0.683;

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while(WiFi.status() != WL_CONNECTED) delay(500);
  mqtt.setServer(MQTT_HOST, 1883);
  while(!mqtt.connected()) mqtt.connect("energy-node");
}

void loop() {
  int raw = analogRead(ADC_PIN);
  float Irms = (raw / 4095.0 - 0.5) * 3.3 * CAL_A_PER_ADC;
  char payload[100];
  sprintf(payload, "{\"Irms\":%.3f}", Irms);
  mqtt.publish("home/energy/node1/c1", payload);
  delay(1000);
}

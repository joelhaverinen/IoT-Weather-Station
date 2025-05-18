#include <WiFiNINA.h>
#include "DHT.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// WiFi-tiedot
#define WIFI_SSID     "xxxx"
#define WIFI_PASS     "xxxx"

// Adafruit IO -tiedot
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "xxxx"
#define AIO_KEY         "xxxx"

// MQTT-asiakas
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Lämpötila feed
Adafruit_MQTT_Publish temperatureFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperature");

// DHT-anturi
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  delay(10);
  dht.begin();

  // Yhdistä WiFiin
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
}

void loop() {
  // Yhdistä MQTT:hen
  if (!mqtt.connected()) {
    reconnect();
  }
  mqtt.processPackets(10000);
  
  // Lue lämpötila
  float temp = dht.readTemperature();
  if (isnan(temp)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Sending temperature: ");
  Serial.println(temp);
  temperatureFeed.publish(temp);

  delay(60000); // lähetä 60 sek välein
}

void reconnect() {
  int8_t ret;
  while ((ret = mqtt.connect()) != 0) {
    mqtt.disconnect();
    delay(5000);
  }
}

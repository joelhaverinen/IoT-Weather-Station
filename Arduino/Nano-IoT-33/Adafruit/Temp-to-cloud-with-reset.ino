#include <WiFiNINA.h>
#include "DHT.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// WiFi credentials
#define WIFI_SSID     "xxxx"
#define WIFI_PASS     "xxxx"

// Adafruit IO credentials
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "xxxx"
#define AIO_KEY         "xxxx"

// MQTT client
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Temperature feed
Adafruit_MQTT_Publish temperatureFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperature");

// DHT sensor
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Timer for reset
unsigned long resetInterval = 900000; // 15 min = 900,000 ms
unsigned long startTime = 0;

void hardResetWithLED() {
  Serial.println("Reset: blinking LED...");
  pinMode(LED_BUILTIN, OUTPUT);
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
  }
  delay(300);
  NVIC_SystemReset();
}

void setup() {
  Serial.begin(9600);
  delay(10);
  dht.begin();

  pinMode(LED_BUILTIN, OUTPUT);

  // Start the timer
  startTime = millis();

  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
}

void loop() {
  // Check if it's time to reset
  if (millis() - startTime >= resetInterval) {
    Serial.println("15 minutes passed – performing reset.");
    hardResetWithLED();
  }

  // Connect to MQTT
  if (!mqtt.connected()) {
    reconnect();
  }
  mqtt.processPackets(10000);
  
  // Read temperature
  float temp = dht.readTemperature();
  if (isnan(temp)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Sending temperature: ");
  Serial.println(temp);
  temperatureFeed.publish(temp);

  delay(60000); // send every 60 seconds
}

void reconnect() {
  int8_t ret;
  while ((ret = mqtt.connect()) != 0) {
    mqtt.disconnect();
    delay(5000);
  }
}

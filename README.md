
## IoT Weather Station

This Arduino project reads temperature data from a DHT22 sensor and uploads it to Adafruit IO using the MQTT protocol over WiFi. To ensure long-term stability, the device performs a hard reset every 15 minutes.

## Features

- WiFi connectivity via WiFiNINA
- Temperature readings from DHT22
- MQTT publishing to Adafruit IO
- Automatic system reset every 15 minutes to prevent hangs or connection drops


## Hardware

- Arduino Nano 33 IoT  
- DHT22 Temperature Sensor  
- WiFi network access  
- Adafruit IO account

## Installation

Install the following libraries via Arduino Library Manager:
   - `WiFiNINA`
   - `DHT sensor library`
   - `Adafruit MQTT Library`
     
Update the following constants in the code:
   #define WIFI_SSID     "your-ssid"
   #define WIFI_PASS     "your-password"
   #define AIO_USERNAME  "your-adafruit-username"
   #define AIO_KEY       "your-adafruit-key"

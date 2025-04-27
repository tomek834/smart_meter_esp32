# Smart Meter ESP32 Project

A smart three-phase energy monitor based on **ESP32** and **ATM90E36** chip, with Wi-Fi and MQTT connectivity, RGB LED status indication, web configuration interface, and OTA (Over-the-Air) updates.

## Features

- **Three-phase energy monitoring** using ATM90E36 chip (Voltage, Current, Active/Reactive/Apparent Power, Power Factor).
- **Wi-Fi connectivity**:
  - Connects to a configured Wi-Fi network.
  - If connection fails, switches to **Access Point** mode with web configuration (secured with password).
- **MQTT publishing**:
  - Publishes measurements periodically.
  - Supports dynamic MQTT Client ID (based on Chip ID)(TODO).
  - Configurable measurement interval.
  - Optional **MQTT over TLS** (TODO).
- **RGB Status LED**:
  - Indicates system states (Booting, Wi-Fi connecting, Wi-Fi connected, MQTT connected, OTA in progress, Error).
- **Over-the-Air updates (OTA)**:
  - Firmware updates over Wi-Fi.
  - Error handling during OTA.
  - Password protection (TODO)
- **Web Server Configuration Panel**:
  - Configure Wi-Fi, MQTT, measurement interval, LED brightness.
  - HTTP Basic Authentication (TODO).
- **Watchdog timer support** (TODO).
- **Neutral wire monitoring**:
  - Monitor and publish neutral current.
- **Persistent configuration storage** using ESP32 NVS.
- **Serial Console Configuration** for initial setup or emergency access.

## TODO 

- Dynamic MQTT Client ID
- Add TLS support for MQTT communication.
- OTA password protection
- Implement HTTP Basic Auth for web interface.
- Watchdog
- Add Home Assistant MQTT Auto-Discovery.
- Dynamic configuration reload without full restart.
- Optional Bluetooth configuration mode.
- Add docker container with mqtt broker, database and grafana
- Hardware redesign (PCB and DIN enclosure)


## Requirements

- ESP32-S3 based board.
- ATM90E36 Energy Monitoring chip.
- Arduino IDE or PlatformIO.
- Required Libraries:
  - `ESPAsyncWebServer`
  - `AsyncTCP`
  - `PubSubClient`
  - `ArduinoOTA`
  - `FastLED`

## Getting Started

1. Flash the firmware to the ESP32.
2. If Wi-Fi is not configured:
   - The device starts an Access Point `SmartMeter` with password `smartmeter123`.
   - Connect and configure the device via the web panel (default ip 192.168.4.1).
   - Alternatively configure device via usb serial (57600 baud rate).
3. Monitor the measurements via MQTT broker.

---


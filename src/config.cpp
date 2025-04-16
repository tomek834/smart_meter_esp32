#include "config.h"
#include <Preferences.h>
#include <WiFi.h>
#include "statusled.h"

Preferences preferences;
Config config;

void showConfig() {
    Serial.println("\n--- Current Configuration ---");
    Serial.print("WiFi SSID: ");
    Serial.println(config.ssid);
    Serial.print("MQTT Server: ");
    Serial.println(config.mqtt_server);
    Serial.print("MQTT Port: ");
    Serial.println(config.mqtt_port);
    Serial.print("Measurement Interval (ms): ");
    Serial.println(config.measurement_interval);
    Serial.print("LED Brightness: ");
    Serial.println(config.led_brightness);
    Serial.print("WiFi Status: ");
    Serial.println(WiFi.status() == WL_CONNECTED ? "Connected" : "Disconnected");
    if (WiFi.status() == WL_CONNECTED) {
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
        Serial.print("RSSI (Signal Strength): ");
        Serial.print(WiFi.RSSI());
        Serial.println(" dBm");
    }
    Serial.println("------------------------------\n");
}

void configureWiFi() {
    Serial.println("Enter WiFi SSID:");
    while (Serial.available() == 0) {}
    String ssid = Serial.readStringUntil('\n');
    ssid.trim();
    ssid.toCharArray(config.ssid, sizeof(config.ssid));

    Serial.println("Enter WiFi Password:");
    while (Serial.available() == 0) {}
    String password = Serial.readStringUntil('\n');
    password.trim();
    password.toCharArray(config.password, sizeof(config.password));

    saveConfig();
    Serial.println("WiFi configuration saved.\n");
}

void configureMQTT() {
    Serial.println("Enter MQTT Server:");
    while (Serial.available() == 0) {}
    String mqtt_server = Serial.readStringUntil('\n');
    mqtt_server.trim();
    mqtt_server.toCharArray(config.mqtt_server, sizeof(config.mqtt_server));

    Serial.println("Enter MQTT Port:");
    while (Serial.available() == 0) {}
    String mqtt_port = Serial.readStringUntil('\n');
    config.mqtt_port = mqtt_port.toInt();

    saveConfig();
    Serial.println("MQTT configuration saved.\n");
}

void configureMeasurementInterval() {
    Serial.println("Enter measurement interval in milliseconds:");
    while (Serial.available() == 0) {}
    String interval = Serial.readStringUntil('\n');
    config.measurement_interval = interval.toInt();

    saveConfig();
    Serial.println("Measurement interval saved.\n");
}

void configureBrightness() {
    Serial.println("Enter LED brightness (0-255):");
    while (Serial.available() == 0) {}
    String brightness = Serial.readStringUntil('\n');
    config.led_brightness = brightness.toInt();

    saveConfig();
    setLEDBrightness(config.led_brightness);  // Apply immediately
    Serial.println("LED brightness saved and applied.\n");
}

void configureSerial() {
    while (true) {
        Serial.println("\n--- Smart Meter Configuration ---");
        Serial.println("1. Configure WiFi");
        Serial.println("2. Configure MQTT");
        Serial.println("3. Configure measurement interval");
        Serial.println("4. Show current configuration");
        Serial.println("5. Configure LED brightness");
        Serial.println("6. Exit configuration");
        Serial.println("Enter option:");

        while (Serial.available() == 0) {}
        String option = Serial.readStringUntil('\n');
        option.trim();

        if (option == "1") {
            configureWiFi();
        } else if (option == "2") {
            configureMQTT();
        } else if (option == "3") {
            configureMeasurementInterval();
        } else if (option == "4") {
            showConfig();
        } else if (option == "5") {
            configureBrightness();
        } else if (option == "6") {
            Serial.println("Exiting configuration mode.\n");
            break;
        } else {
            Serial.println("Invalid option. Please try again.\n");
        }
    }
}

void loadConfig() {
    preferences.begin("smartmeter", true);
    String ssid = preferences.getString("ssid", "");
    String password = preferences.getString("password", "");
    String mqtt_server = preferences.getString("mqtt_server", "");
    int mqtt_port = preferences.getInt("mqtt_port", 1883);
    int measurement_interval = preferences.getInt("interval", 1000);
    int led_brightness = preferences.getInt("led_brightness", 50);
    preferences.end();

    ssid.toCharArray(config.ssid, sizeof(config.ssid));
    password.toCharArray(config.password, sizeof(config.password));
    mqtt_server.toCharArray(config.mqtt_server, sizeof(config.mqtt_server));
    config.mqtt_port = mqtt_port;
    config.measurement_interval = measurement_interval;
    config.led_brightness = led_brightness;
    setLEDBrightness(config.led_brightness);  // Apply on load
}

void saveConfig() {
    preferences.begin("smartmeter", false);
    preferences.putString("ssid", config.ssid);
    preferences.putString("password", config.password);
    preferences.putString("mqtt_server", config.mqtt_server);
    preferences.putInt("mqtt_port", config.mqtt_port);
    preferences.putInt("interval", config.measurement_interval);
    preferences.putInt("led_brightness", config.led_brightness);
    preferences.end();
}

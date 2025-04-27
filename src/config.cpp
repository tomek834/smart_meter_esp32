#include "config.h"
#include <Preferences.h>
#include <WiFi.h>
#include "statusled.h"
#include "powerMeasurement.h"

#define RESET_PIN 0 // GPIO0 (BOOT button)

Preferences preferences;
Config config;

bool printMeasurementsToSerial = true; 

void showConfig() {
    Serial.println("\n--- Current Configuration ---");
    Serial.print("WiFi SSID: ");
    Serial.println(config.ssid);
    Serial.print("MQTT Server: ");
    Serial.println(config.mqtt_server);
    Serial.print("MQTT Port: ");
    Serial.println(config.mqtt_port);
    Serial.print("Measurement Interval (ms): ");
    Serial.println(config.meas_itv);
    Serial.print("LED Brightness: ");
    Serial.println(config.led_brightness);
    Serial.print("Neutral Monitoring: ");
    Serial.println(config.n_monitor ? "Enabled" : "Disabled");
    Serial.print("Printing to serial: ");
    Serial.println(config.printToSrl ? "Enabled" : "Disabled");
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
    config.meas_itv = interval.toInt();

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

void configureNeutralMonitoring() {
    config.n_monitor = !config.n_monitor;
    enableNeutralMonitoring(config.n_monitor);
    saveConfig();
    Serial.print("Neutral monitoring ");
    Serial.println(config.n_monitor ? "enabled." : "disabled.");
}

void configureSerialOutput() {
    config.printToSrl = !config.printToSrl;
    saveConfig();
    Serial.print("Serial output of measurements ");
    Serial.println(config.printToSrl ? "enabled." : "disabled.");
}

void configureSerial() {
    while (true) {
        Serial.println("\n--- Smart Meter Configuration ---");
        Serial.println("1. Configure WiFi");
        Serial.println("2. Configure MQTT");
        Serial.println("3. Configure measurement interval");
        Serial.println("4. Show current configuration");
        Serial.println("5. Configure LED brightness");
        Serial.println("6. Toggle neutral monitoring");
        Serial.println("7. Toggle serial output of measurements");
        Serial.println("8. Exit configuration");
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
            configureNeutralMonitoring();
        } else if (option == "7") {
            configureSerialOutput();
        } else if (option == "8") {
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
    bool n_monitor = preferences.getBool("n_monitor", false);
    bool printToSrl = preferences.getBool("printToSrl", false);
    preferences.end();

    ssid.toCharArray(config.ssid, sizeof(config.ssid));
    password.toCharArray(config.password, sizeof(config.password));
    mqtt_server.toCharArray(config.mqtt_server, sizeof(config.mqtt_server));
    config.mqtt_port = mqtt_port;
    config.meas_itv = measurement_interval;
    config.led_brightness = led_brightness;
    config.n_monitor = n_monitor;
    config.printToSrl = printToSrl;

    setLEDBrightness(config.led_brightness);
    enableNeutralMonitoring(config.n_monitor);
    
}

void saveConfig() {
    preferences.begin("smartmeter", false);
    preferences.putString("ssid", config.ssid);
    preferences.putString("password", config.password);
    preferences.putString("mqtt_server", config.mqtt_server);
    preferences.putInt("mqtt_port", config.mqtt_port);
    preferences.putInt("interval", config.meas_itv);
    preferences.putInt("led_brightness", config.led_brightness);
    preferences.putBool("n_monitor", config.n_monitor);
    preferences.putBool("printToSrl", config.printToSrl);
    preferences.end();
}

void clearConfig() {
    preferences.begin("smartmeter", false);
    preferences.clear(); 
    preferences.end(); 
    Serial.println("All configuration settings have been cleared.");
}


void checkResetPin() {
    pinMode(RESET_PIN, INPUT_PULLUP); 
    if (digitalRead(RESET_PIN) == LOW) { 
        Serial.println("Reset pin triggered. Resetting configuration...");
        resetConfig();
        ESP.restart(); 
    }
}

void resetConfig() {
    Serial.println("Clearing stored configuration...");
    clearConfig(); 
    Serial.println("Configuration reset to defaults.");
}
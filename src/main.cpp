#include "config.h"
#include "mywifi.h"
#include "mqtt.h"
#include "ota.h"
#include "powerMeasurement.h"

unsigned long lastMeasurementTime = 0;

void checkSerialCommands() {
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        command.trim();
        if (command == "config") {
            configureSerial();
        }
    }
}

void setup() {
    Serial.begin(115200);
    loadConfig();

    if (strlen(config.ssid) == 0 || strlen(config.password) == 0 || strlen(config.mqtt_server) == 0 || config.mqtt_port == 0 || config.measurement_interval == 0) {
        Serial.println("Configuration incomplete. Entering configuration mode.");
        configureSerial();
    }

    setupMyWiFi();
    setupMQTT();
    setupOTA();
    setupPowerMeasurement();
}

void loop() {
    checkSerialCommands();
    handleMQTT();
    handleOTA();
    updateMyWiFiStatus();

    unsigned long currentMillis = millis();
    if (currentMillis - lastMeasurementTime >= config.measurement_interval) {
        lastMeasurementTime = currentMillis;
        readPowerMeasurements();
    }
}
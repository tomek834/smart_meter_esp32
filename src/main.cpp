#include "config.h"
#include "mywifi.h"
#include "mqtt.h"
#include "ota.h"
#include "powerMeasurement.h"
#include <WiFi.h>
#include "statusled.h"

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
    
    Serial.begin(57600); //1152000 might be to fast for some usb-serial adapters
    //while (!Serial) delay(10);
    setupStatusLED();
    Serial.println("Press 'config' to enter setup mode...");
    unsigned long startTime = millis();
    while (millis() - startTime < 5000) { // Wait for 5 seconds for user input
        if (Serial.available()) {
            String command = Serial.readStringUntil('\n');
            command.trim();
            if (command == "config") {
                Serial.println("Entering configuration mode...");
                configureSerial();
                break;
            }
        }
    }
    
    
    loadConfig();

    //allow empty password?
    if (strlen(config.ssid) == 0 || strlen(config.password) == 0 || strlen(config.mqtt_server) == 0 || config.mqtt_port == 0 || config.meas_itv == 0) {
        Serial.println("Configuration incomplete. Entering configuration mode.");
        startAPMode();
        configureSerial();
    }

    setupMyWiFi();

    if (WiFi.status() == WL_CONNECTED) {
        Serial.print("Connected to WiFi. IP address: ");
        Serial.println(WiFi.localIP());
        updateStatusLED(STATUS_WIFI_CONNECTED);
    } else {
        Serial.println("Failed to connect to WiFi. Starting AP mode...");
        startAPMode();
    }


    setupMQTT();
    setupOTA();
    setupPowerMeasurement();

    if (isATM90E36Connected()) {
        Serial.println("ATM90E36 is responding correctly via SPI.");
    } else {
        Serial.println("Failed to detect ATM90E36. Check wiring and SPI settings.");
    }

}

void loop() {
    checkResetPin();
    
    checkSerialCommands();
    handleMQTT();
    handleOTA();
    if (WiFi.status() != WL_CONNECTED) {
        updateStatusLED(STATUS_WIFI_CONNECTING);
    } else if (!isMQTTConnected()) {
        updateStatusLED(STATUS_WIFI_CONNECTED);
    } else {
        updateStatusLED(STATUS_MQTT_CONNECTED);
    }



    unsigned long currentMillis = millis();
    if (currentMillis - lastMeasurementTime >= config.meas_itv) {
        lastMeasurementTime = currentMillis;
        if (config.printToSrl) {
            readPowerMeasurements();
        }
    }
}
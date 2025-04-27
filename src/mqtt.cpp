#include "mqtt.h"
#include "config.h"
#include "powerMeasurement.h"
#include <PubSubClient.h>
#include <WiFi.h>

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastPublishTime = 0;

void reconnect() {
    int retryCount = 0;
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (client.connect("ESP32Client")) {
            Serial.println("connected");
            client.publish("smartmeter/status", "ESP32 connected");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
            retryCount++; 
            if (retryCount >= 5) { 
                Serial.println("Failed to connect after 5 attempts. Entering configuration mode...");
                configureSerial(); 
                break;
            }
        }
    }
}

void setupMQTT() {
    client.setServer(config.mqtt_server, config.mqtt_port);
}

bool isMQTTConnected() {
    return client.connected();
}


void publishMeasurement(const char* topic, float value) {
    char payload[32];
    snprintf(payload, sizeof(payload), "%.2f", value);
    client.publish(topic, payload);
    if (config.printToSrl) {
        Serial.print("Published ");
        Serial.print(topic);
        Serial.print(": ");
        Serial.println(payload);
    }
    
}

void handleMQTT() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    unsigned long now = millis();
    if (now - lastPublishTime > config.meas_itv) {
        lastPublishTime = now;

        // Faza A
        publishMeasurement("smartmeter/voltage_a", getVoltageA());
        publishMeasurement("smartmeter/current_a", getCurrentA());
        publishMeasurement("smartmeter/active_power_a", getActivePowerA());
        publishMeasurement("smartmeter/reactive_power_a", getReactivePowerA());
        publishMeasurement("smartmeter/apparent_power_a", getApparentPowerA());
        publishMeasurement("smartmeter/power_factor_a", getPowerFactorA());

        // Faza B
        publishMeasurement("smartmeter/voltage_b", getVoltageB());
        publishMeasurement("smartmeter/current_b", getCurrentB());
        publishMeasurement("smartmeter/active_power_b", getActivePowerB());
        publishMeasurement("smartmeter/reactive_power_b", getReactivePowerB());
        publishMeasurement("smartmeter/apparent_power_b", getApparentPowerB());
        publishMeasurement("smartmeter/power_factor_b", getPowerFactorB());

        // Faza C
        publishMeasurement("smartmeter/voltage_c", getVoltageC());
        publishMeasurement("smartmeter/current_c", getCurrentC());
        publishMeasurement("smartmeter/active_power_c", getActivePowerC());
        publishMeasurement("smartmeter/reactive_power_c", getReactivePowerC());
        publishMeasurement("smartmeter/apparent_power_c", getApparentPowerC());
        publishMeasurement("smartmeter/power_factor_c", getPowerFactorC());

        // Przewód neutralny (jeśli włączone)
        if (config.n_monitor) {
            publishMeasurement("smartmeter/current_n", getCurrentN());
        }
    }
}

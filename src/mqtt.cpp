#include "mqtt.h"
#include "config.h"
#include "powerMeasurement.h"
#include <PubSubClient.h>
#include <WiFi.h>

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastPublishTime = 0;

void reconnect() {
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
        }
    }
}

void setupMQTT() {
    client.setServer(config.mqtt_server, config.mqtt_port);
}

void publishMeasurement(const char* topic, float value) {
    char payload[32];
    snprintf(payload, sizeof(payload), "%.2f", value);
    client.publish(topic, payload);
    Serial.print("Published ");
    Serial.print(topic);
    Serial.print(": ");
    Serial.println(payload);
}

void handleMQTT() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    unsigned long now = millis();
    if (now - lastPublishTime > config.measurement_interval) {
        lastPublishTime = now;

        publishMeasurement("smartmeter/active_power", getActivePower());
        publishMeasurement("smartmeter/reactive_power", getReactivePower());
        publishMeasurement("smartmeter/apparent_power", getApparentPower());
        publishMeasurement("smartmeter/voltage", getVoltage());
        publishMeasurement("smartmeter/current", getCurrent());
        publishMeasurement("smartmeter/power_factor", getPowerFactor());
    }
}

bool isMQTTConnected() {
    return client.connected();
}

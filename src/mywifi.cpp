#include "mywifi.h"
#include "config.h"
#include <WiFi.h>

void setupMyWiFi() {
    if (strlen(config.ssid) == 0 || strlen(config.password) == 0) {
        Serial.println("WiFi credentials not set. Please configure via serial.");
        configureWiFi();
        return;
    }

    Serial.print("Connecting to WiFi: ");
    Serial.println(config.ssid);

    WiFi.begin(config.ssid, config.password);
    int retries = 0;
    while (WiFi.status() != WL_CONNECTED && retries < 20) {
        delay(500);
        Serial.print(".");
        retries++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi connected!");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("\nFailed to connect to WiFi.");
    }
}

void updateMyWiFiStatus() {
    // TODO : status polaczenia wifi na led rgb
}

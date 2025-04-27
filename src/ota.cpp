#include <WiFi.h>
#include "ota.h"
#include <ArduinoOTA.h>

void setupOTA() {
    ArduinoOTA.setHostname("smartmeter");

    ArduinoOTA
      .onStart([]() {
        Serial.println("OTA Start");
      })
      .onEnd([]() {
        Serial.println("\nOTA End");
      })
      .onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("OTA Progress: %u%%\r", (progress * 100) / total);
      })
      .onError([](ota_error_t error) {
        Serial.printf("OTA Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) {
            Serial.println("Authentication Failed");
        } else if (error == OTA_BEGIN_ERROR) {
            Serial.println("Begin Failed");
        } else if (error == OTA_CONNECT_ERROR) {
            Serial.println("Connection Failed");
        } else if (error == OTA_RECEIVE_ERROR) {
            Serial.println("Receive Failed");
        } else if (error == OTA_END_ERROR) {
            Serial.println("End Failed");
        }
      });
  
    ArduinoOTA.begin();
    Serial.println("OTA Ready");
}

void handleOTA() {
    ArduinoOTA.handle();
}
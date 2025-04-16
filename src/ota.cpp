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
        Serial.println("OTA End");
      })
      .onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("OTA Progress: %u%%\r", (progress * 100) / total);
      })
      .onError([](ota_error_t error) {
        Serial.printf("OTA Error[%u]: ", error);
      });
  
    ArduinoOTA.begin();
    Serial.println("OTA Ready");
  }

  void handleOTA() {
    ArduinoOTA.handle();
}
#ifndef STATUSLED_H
#define STATUSLED_H

#include <stdint.h>

enum SystemStatus {
    STATUS_BOOTING,
    STATUS_WIFI_CONNECTING,
    STATUS_WIFI_CONNECTED,
    STATUS_MQTT_CONNECTED,
    STATUS_OTA_IN_PROGRESS,
    STATUS_ERROR
};

void setupStatusLED();
void updateStatusLED(SystemStatus status);
void setLEDBrightness(int brightness);
void handleStatusLEDBlink();

#endif
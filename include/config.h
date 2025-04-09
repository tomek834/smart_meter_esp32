#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

struct Config {
    char ssid[32];
    char password[32];
    char mqtt_server[64];
    uint16_t mqtt_port;
    uint32_t measurement_interval; // miliseconds
};

extern Config config;

void loadConfig();
void saveConfig();
void configureSerial();
void configureWiFi();
void configureMQTT();
void configureMeasurementInterval();
void showConfig();


#endif
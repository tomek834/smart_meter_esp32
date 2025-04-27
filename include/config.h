#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

struct Config {
    char ssid[32];
    char password[32];
    char mqtt_server[64];
    uint16_t mqtt_port;
    uint32_t meas_itv; // milliseconds
    int led_brightness;
    bool n_monitor;
    bool printToSrl;
};

extern Config config;


void loadConfig();
void saveConfig();
void configureSerial();
void configureWiFi();
void configureMQTT();
void configureMeasurementInterval();
void configureBrightness();
void configureNeutralMonitoring();
void showConfig();
void checkResetPin();
void clearConfig();
void resetConfig();

#endif
#include <FastLED.h>
#include <Arduino.h>
#include "statusled.h"

#define DATA_PIN 48
#define NUM_LEDS 1

CRGB leds[NUM_LEDS];

int currentBrightness = 50;
SystemStatus currentStatus = STATUS_BOOTING;
unsigned long lastBlinkTime = 0;
bool blinkState = false;

void setupStatusLED() {
    FastLED.addLeds<SK6812, DATA_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(currentBrightness);
    updateStatusLED(currentStatus);
}

void setLEDBrightness(int brightness) {
    currentBrightness = constrain(brightness, 0, 255);
    FastLED.setBrightness(currentBrightness);
    updateStatusLED(currentStatus);
}

void updateStatusLED(SystemStatus status) {
    currentStatus = status;
    blinkState = false;
    handleStatusLEDBlink();
}

void handleStatusLEDBlink() {
    static uint8_t r, g, b;
    uint8_t brightness = currentBrightness;

    if (currentStatus == STATUS_WIFI_CONNECTING || currentStatus == STATUS_OTA_IN_PROGRESS) {
        unsigned long now = millis();
        if (now - lastBlinkTime >= 500) {
            lastBlinkTime = now;
            blinkState = !blinkState;
        }
    } else {
        blinkState = true;
    }

    if (blinkState) {
        switch (currentStatus) {
            case STATUS_BOOTING:
                r = brightness; g = brightness; b = 0;     // Yellow
                break;
            case STATUS_WIFI_CONNECTING:
                r = 0; g = 0; b = brightness;             // Blue
                break;
            case STATUS_WIFI_CONNECTED:
                r = 0; g = brightness / 2; b = brightness;// Cyan
                break;
            case STATUS_MQTT_CONNECTED:
                r = 0; g = brightness; b = 0;             // Green
                break;
            case STATUS_OTA_IN_PROGRESS:
                r = brightness; g = brightness / 2; b = 0;// Orange
                break;
            case STATUS_ERROR:
                r = brightness; g = 0; b = 0;             // Red
                break;
            default:
                r = g = b = 0;                            // Off
                break;
        }
    } else {
        r = g = b = 0; // OFF phase of blink
    }

    leds[0] = CRGB(r, g, b);
    FastLED.show();
}

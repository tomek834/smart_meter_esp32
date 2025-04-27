
#include <Arduino.h>
#include <SPI.h>
#include "ATM90E36.h"
#include "powerMeasurement.h"

#define PIN_MOSI 11
#define PIN_MISO 13
#define PIN_SCLK 12
#define PIN_CS   10

SPIClass hspi(HSPI);

ATM90E36 atm90e36(PIN_CS);
bool monitorNeutral = false;

void enableNeutralMonitoring(bool enable) {
    monitorNeutral = enable;
}

bool isNeutralMonitoringEnabled() {
    return monitorNeutral;
}

void setupPowerMeasurement() {
    hspi.begin(PIN_SCLK, PIN_MISO, PIN_MOSI, PIN_CS);
    atm90e36.begin();
    Serial.println("ATM90E36 initialized using HSPI (SPI2). Three-phase mode with optional N.");
}

void readPowerMeasurements() {
    Serial.print("Voltage A: "); Serial.println(getVoltageA());
    Serial.print("Voltage B: "); Serial.println(getVoltageB());
    Serial.print("Voltage C: "); Serial.println(getVoltageC());

    Serial.print("Current A: "); Serial.println(getCurrentA());
    Serial.print("Current B: "); Serial.println(getCurrentB());
    Serial.print("Current C: "); Serial.println(getCurrentC());
    if (monitorNeutral) {
        Serial.print("Current N: "); Serial.println(getCurrentN());
    }

    Serial.print("Active Power A: "); Serial.println(getActivePowerA());
    Serial.print("Active Power B: "); Serial.println(getActivePowerB());
    Serial.print("Active Power C: "); Serial.println(getActivePowerC());

    Serial.print("Reactive Power A: "); Serial.println(getReactivePowerA());
    Serial.print("Reactive Power B: "); Serial.println(getReactivePowerB());
    Serial.print("Reactive Power C: "); Serial.println(getReactivePowerC());

    Serial.print("Apparent Power A: "); Serial.println(getApparentPowerA());
    Serial.print("Apparent Power B: "); Serial.println(getApparentPowerB());
    Serial.print("Apparent Power C: "); Serial.println(getApparentPowerC());

    Serial.print("Power Factor A: "); Serial.println(getPowerFactorA());
    Serial.print("Power Factor B: "); Serial.println(getPowerFactorB());
    Serial.print("Power Factor C: "); Serial.println(getPowerFactorC());
}

bool isATM90E36Connected() {
    uint16_t status0 = atm90e36.GetSysStatus0();
    uint16_t status1 = atm90e36.GetSysStatus1();

    Serial.print("System Status 0 Register: 0x");
    Serial.println(status0, HEX);
    Serial.print("System Status 1 Register: 0x");
    Serial.println(status1, HEX);

    // error if status0 or status1 is 0x0000 or 0xFFFF
    bool validStatus0 = (status0 != 0x0000 && status0 != 0xFFFF);
    bool validStatus1 = (status1 != 0x0000 && status1 != 0xFFFF);

    return validStatus0 || validStatus1;
}



float getVoltageA()       { return atm90e36.GetLineVoltageA(); }
float getVoltageB()       { return atm90e36.GetLineVoltageB(); }
float getVoltageC()       { return atm90e36.GetLineVoltageC(); }

float getCurrentA()       { return atm90e36.GetLineCurrentA(); }
float getCurrentB()       { return atm90e36.GetLineCurrentB(); }
float getCurrentC()       { return atm90e36.GetLineCurrentC(); }
float getCurrentN()       { return atm90e36.GetLineCurrentN(); }

float getActivePowerA()   { return atm90e36.GetActivePowerA(); }
float getActivePowerB()   { return atm90e36.GetActivePowerB(); }
float getActivePowerC()   { return atm90e36.GetActivePowerC(); }

float getReactivePowerA() { return atm90e36.GetReactivePowerA(); }
float getReactivePowerB() { return atm90e36.GetReactivePowerB(); }
float getReactivePowerC() { return atm90e36.GetReactivePowerC(); }

float getApparentPowerA() { return atm90e36.GetApparentPowerA(); }
float getApparentPowerB() { return atm90e36.GetApparentPowerB(); }
float getApparentPowerC() { return atm90e36.GetApparentPowerC(); }

float getPowerFactorA()   { return atm90e36.GetPowerFactorA(); }
float getPowerFactorB()   { return atm90e36.GetPowerFactorB(); }
float getPowerFactorC()   { return atm90e36.GetPowerFactorC(); }

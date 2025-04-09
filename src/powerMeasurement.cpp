#include "powerMeasurement.h"

// Test values for simulation
float activePower = 123.45;
float reactivePower = 67.89;
float apparentPower = 150.12;
float voltage = 230.0;
float current = 5.2;
float powerFactor = 0.95;

void setupPowerMeasurement() {
    // init atm90e36
}

void readPowerMeasurements() {
    // read atm90e36 values
    // test values for simulation
    activePower += 0.1; 
}

float getActivePower() { return activePower; }
float getReactivePower() { return reactivePower; }
float getApparentPower() { return apparentPower; }
float getVoltage() { return voltage; }
float getCurrent() { return current; }
float getPowerFactor() { return powerFactor; }

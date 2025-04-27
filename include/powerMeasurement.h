#ifndef POWERMEASUREMENT_H
#define POWERMEASUREMENT_H

void setupPowerMeasurement();
void readPowerMeasurements();

float getVoltageA();
float getVoltageB();
float getVoltageC();
float getCurrentA();
float getCurrentB();
float getCurrentC();
float getCurrentN();
float getActivePowerA();
float getActivePowerB();
float getActivePowerC();
float getReactivePowerA();
float getReactivePowerB();
float getReactivePowerC();
float getApparentPowerA();
float getApparentPowerB();
float getApparentPowerC();
float getPowerFactorA();
float getPowerFactorB();
float getPowerFactorC();

void enableNeutralMonitoring(bool enable);
bool isNeutralMonitoringEnabled();
bool isATM90E36Connected();
#endif
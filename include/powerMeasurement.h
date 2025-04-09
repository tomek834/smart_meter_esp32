#ifndef POWER_MEASUREMENT_H
#define POWER_MEASUREMENT_H

void setupPowerMeasurement();
void readPowerMeasurements();


float getActivePower();
float getReactivePower();
float getApparentPower();
float getVoltage();
float getCurrent();
float getPowerFactor();

#endif

#include "General_Utils.h"

#ifndef Temperature_Sensor_Utils
#define Temperature_Sensor_Utils

bool shouldSendLowTempAlert(unsigned long espUpTime,float currentTemperature, RollingAverage* TemperatureRollingAverage);
#endif
#include "Temperature_Sensor_Utils.h"

unsigned long lastSampleTime = 0;  // Define the variable
const unsigned long sampleInterval = 5000;
float roomTempThreshold = 22.0;  // Define the variable
bool coldWarning = false;  // Define the variable


/*
* Decide if we need to send an alert message 
* to the careers
*
*/
bool shouldSendLowTempAlert(unsigned long currentMillis,float currentTemperature,RollingAverage* TemperatureRollingAverage)
{
    if (currentMillis - lastSampleTime >= sampleInterval) {
        lastSampleTime = currentMillis;
        if((currentTemperature < roomTempThreshold) && coldWarning == false)
        {
            TemperatureRollingAverage->pushNew(currentTemperature);
            float currentAverage = TemperatureRollingAverage->getAverage();
            /*
             * 200 is an impossible number to reach so instead 
             * we use it to show we have not collected enough temperature readings.
            */
            if(currentAverage != 200.f || currentAverage < roomTempThreshold){
                coldWarning = true;
                return true;
            }

        }
        if(currentTemperature > roomTempThreshold)
        {
        coldWarning = false;
        }
    }
    return false;

}
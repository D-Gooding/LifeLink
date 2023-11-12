#include "General_Utils.h"
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <regex>

  
void RollingAverage::pushNew(float val)
{
    if(TemperatureRollingAverage.size() > MAX_QUEUE_SIZE){
        TemperatureRollingAverage.pop_front();
    }
    TemperatureRollingAverage.push_back(val);
}

float RollingAverage::getAverage()
{
    /*
     * 200 is an impossible number to reach so instead 
     * we use it to show we have not collected enough temperature readings.
     */
    
    if(TemperatureRollingAverage.size() != MAX_QUEUE_SIZE){
        return 200.f;
    }
    float total = 0;
    for (const auto& element : TemperatureRollingAverage) {
        total+= element;
    }
    return (total/MAX_QUEUE_SIZE);
}




bool  UtilsisStringEmpty(const char *str) 
{
    return str == NULL || *str == '\0';
}


const char* UtilstoUpperCase(const char* str)
{
      static char result[BUFSIZ]; // Assuming a reasonable buffer size
    int length = strlen(str);

    for (int i = 0; i < length; i++) {
        result[i] = toupper(str[i]);
    }
    result[length] = '\0'; // Null-terminate the new string

    return result;
}

bool isMobileNumber(const char* mobileNumber)
{
    std::regex MobileNumberPattern("^[0-9+]+$");
    return std::regex_match(mobileNumber, MobileNumberPattern);
}
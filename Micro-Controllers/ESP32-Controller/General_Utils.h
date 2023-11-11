#include <deque>

#ifndef General_Utils
#define General_Utils

class RollingAverage {
public:
    RollingAverage(){}
    void pushNew(float value);
    float getAverage();

private:
    std::deque<float> TemperatureRollingAverage;
    int MAX_QUEUE_SIZE = 10;
};

bool UtilsisStringEmpty(const char *str);
const char* UtilstoUpperCase(const char* str);
bool isMobileNumber(const char* str);

#endif
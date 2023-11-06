#include "General_Utils.h"
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <cctype>

bool UtilsisStringEmpty(const char *str) 
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
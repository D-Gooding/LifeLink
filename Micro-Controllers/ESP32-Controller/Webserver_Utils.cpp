#include "Webserver_Utils.h"
#include <cstring>
#include <regex>


const char* ReconstructNumbers(char mobileNumbers[MAX_MOBILE_NUMBERS][MOBILE_NUMBER_LENGTH]) {
  static char reconstructed[256];
  reconstructed[0] = '\0';

  std::regex MobileNumberPattern("^[0-9+]+$");

  for (int i = 0; i < MAX_MOBILE_NUMBERS; i++) {
    if(!std::regex_match(mobileNumbers[i], MobileNumberPattern))
    {
      break;
    }
    strncat(reconstructed, mobileNumbers[i], sizeof(reconstructed) - strlen(reconstructed) - 1);

    if (i < MAX_MOBILE_NUMBERS - 1) {
      strncat(reconstructed, ",", sizeof(reconstructed) - strlen(reconstructed) - 1);
    }
  }

  return reconstructed;
}
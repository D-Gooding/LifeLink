#include <ArduinoJson.h>

#define RXp2 16
#define TXp2 17

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
  pinMode(LED_BUILTIN, OUTPUT);
}

bool isStringEmpty(const char *str) 
{
    return str == NULL || *str == '\0';
}

const char* toUpperCase(const char* str)
{
      static char result[BUFSIZ]; // Assuming a reasonable buffer size
    int length = strlen(str);

    for (int i = 0; i < length; i++) {
        result[i] = toupper(str[i]);
    }
    result[length] = '\0'; // Null-terminate the new string

    return result;
}


bool checkIfNumberValid()
{
  //Check against saved list of numbers
}

void toggleTVON()
{
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(1000);                      // wait for a second
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
    delay(1000); 
}


void decodeTVCommand(const char* message)
{
  if (strncmp(message, "ON", 2) == 0) {
    toggleTVON();
  }
}



void decodeMessage(const char* message)
{
  //Is TV command
  if (strncmp(message, "TV", 2) == 0) {
    decodeTVCommand(message + 3);
  }
}




void loop() {
  if(Serial2.available()){
    DynamicJsonDocument jsonSerialMessage(1024);
    deserializeJson(jsonSerialMessage, Serial2.readString());
    const char* message = jsonSerialMessage["message"];
    if(!isStringEmpty(message))
    {
      Serial.println(toUpperCase(message)); 
      decodeMessage(toUpperCase(message));
    }
  }

}

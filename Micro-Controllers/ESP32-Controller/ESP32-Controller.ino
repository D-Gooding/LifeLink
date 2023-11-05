#include <ArduinoJson.h>
#include <WiFiManager.h>
#include <EEPROM.h>
#include "Webserver_Utils.h"


#define RXp2 16
#define TXp2 17

// defining the bytes were gonna need
#define GSM_TOGGLE_SIZE 1
#define MAX_MOBILE_NUMBERS 5
#define MOBILE_NUMBER_LENGTH 20


bool GSMMode = true;
char mobileNumbers[MAX_MOBILE_NUMBERS][MOBILE_NUMBER_LENGTH];



WiFiManager wm;
WiFiManagerParameter GSMToggleParam; // global param ( for non blocking w params )
WiFiManagerParameter AllowListNumbers;




void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
  pinMode(LED_BUILTIN, OUTPUT);

  // initialize EEPROM with predefined size
  EEPROM.begin(1 + (MAX_MOBILE_NUMBERS * MOBILE_NUMBER_LENGTH ));

  bool res;


  if(EEPROM.read(0) == 1)
  {
    GSMMode = true;
  }else{
    GSMMode = false;
  }

  readMobileNumbersFromEEPROM();

  Serial.print(ReconstructNumbers());


  // ============================== GSM Radio Button select ==============================
  int customFieldLength = 40;
  char custom_radio_str[300];
  sprintf(custom_radio_str, "<br/><label for='GSMToggle'>Toggle GSM Mode</label><br></br><input type='radio' name='customfieldid' value='true' %s> On<br><input type='radio' name='customfieldid' value='false' %s> Off <br>",
        (EEPROM.read(0) == 1 ? "checked" : ""),
        (EEPROM.read(0) == 0 ? "checked" : "")
  );

  new (&GSMToggleParam) WiFiManagerParameter(custom_radio_str); // custom html input
  
  wm.addParameter(&GSMToggleParam);

  // ============================== GSM valid Number ==============================
  new (&AllowListNumbers) WiFiManagerParameter("MobileNumberAllowListId", "Enter allowed number below", ReconstructNumbers(), customFieldLength,"placeholder=\"+44********** \"");
  wm.addParameter(&AllowListNumbers);


  wm.setSaveParamsCallback(saveParamCallback);

  std::vector<const char *> menu = {"wifi","info","param","sep","restart","exit"};
  wm.setMenu(menu);

  // set dark theme
  wm.setClass("invert");



  res = wm.autoConnect("LifeLinkAP","password"); // password protected ap

  

  if(!res) {
      Serial.println("Failed to connect");
      //ESP.restart();
  } 
  else {
      //if you get here you have connected to the WiFi    
      Serial.println("connected...yeey :)");
  }

}

void toggleGSM(bool state)
{
  state ? EEPROM.write(0,1): EEPROM.write(0,0);
  GSMMode = state; 
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

String getParam(String name){
  //read parameter from server, for customhmtl input
  String value;
  if(wm.server->hasArg(name)) {
    value = wm.server->arg(name);
  }
  return value;
}

const char* ReconstructNumbers() {
  static char reconstructed[256];
  reconstructed[0] = '\0';

  for (int i = 0; i < MAX_MOBILE_NUMBERS; i++) {
    strncat(reconstructed, mobileNumbers[i], sizeof(reconstructed) - strlen(reconstructed) - 1);

    if (i < MAX_MOBILE_NUMBERS - 1) {
      strncat(reconstructed, ",", sizeof(reconstructed) - strlen(reconstructed) - 1);
    }
  }

  return reconstructed;
}


void UpdateNumbers(const char* numbers) {
  char* token = strtok((char*)numbers, ",");
  int index = 0;

  while (token != NULL && index < MAX_MOBILE_NUMBERS) {
    strncpy(mobileNumbers[index], token, MOBILE_NUMBER_LENGTH - 1);
    mobileNumbers[index][MOBILE_NUMBER_LENGTH - 1] = '\0'; // Null-terminate the string
    writeNumberToEEPROM(index); // Write each number to EEPROM
    token = strtok(NULL, ",");
    index++;
  }
}

void writeNumberToEEPROM(int index) {
  char buffer[MOBILE_NUMBER_LENGTH];
  strncpy(buffer, mobileNumbers[index], MOBILE_NUMBER_LENGTH - 1);
  buffer[MOBILE_NUMBER_LENGTH - 1] = '\0'; // Null-terminate the string
  EEPROM.put(index * MOBILE_NUMBER_LENGTH, buffer);
  EEPROM.commit();
}

 /*
  * Read From the EEPROM the list of SAVED Numbers.
  */
void readMobileNumbersFromEEPROM() {
  for (int i = 0; i < MAX_MOBILE_NUMBERS; i++) {
    char buffer[MOBILE_NUMBER_LENGTH];
    EEPROM.get(i * MOBILE_NUMBER_LENGTH, buffer);
    if(buffer[0] == '\0'){break;}
    strncpy(mobileNumbers[i], buffer, MOBILE_NUMBER_LENGTH);
    mobileNumbers[i][MOBILE_NUMBER_LENGTH - 1] = '\0'; // Null-terminate the string

     

  }
}



//====================== Calls for On change of setting ======================
void ToggleGSM(bool state)
{
  state ? EEPROM.write(MOBILE_NUMBER_LENGTH * MAX_MOBILE_NUMBERS,1) : EEPROM.write(MOBILE_NUMBER_LENGTH * MAX_MOBILE_NUMBERS,0);
  EEPROM.commit();
  GSMMode = state;
}


void SetAllowNumbers(const char* NumberListAsChar)
{
  UpdateNumbers(NumberListAsChar);
}













//====================== Config Menu Custom Call back ======================
void saveParamCallback(){
  Serial.println("[CALLBACK] saveParamCallback fired");
  Serial.println("PARAM customfieldid = " + getParam("MobileNumberAllowListId"));
  ToggleGSM((getParam("GSMToggle").c_str(),"true") == 0);
  SetAllowNumbers(getParam("MobileNumberAllowListId").c_str());
  
}








void GSMBufferLoop()
{
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

void loop() {
  if(GSMMode){
    GSMBufferLoop();
  }

}

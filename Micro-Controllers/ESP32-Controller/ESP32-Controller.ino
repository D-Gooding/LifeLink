#include <ArduinoJson.h>
#include <WiFiManager.h>
#include <EEPROM.h>
#include "Webserver_Utils.h"
#include "General_Utils.h"


#define RXp2 16
#define TXp2 17

#define TRIGGER_PIN 0

// defining the bytes were gonna need
#define GSM_TOGGLE_SIZE 1

#define AP_NAME "LifeLinkAP"
#define AP_PASSWORD "password"


bool wm_nonblocking = false; // change to true to use non blocking


bool GSMMode = true;
char mobileNumbers[MAX_MOBILE_NUMBERS][MOBILE_NUMBER_LENGTH];
int GSMToggleEPROM = 0;


WiFiManager wm;
WiFiManagerParameter GSMToggleParam; // global param ( for non blocking w params )
WiFiManagerParameter AllowListNumbers;



void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
  pinMode(LED_BUILTIN, OUTPUT);

  // initialize EEPROM with predefined size
  EEPROM.begin(20 + (MAX_MOBILE_NUMBERS * MOBILE_NUMBER_LENGTH ));
  GSMToggleEPROM = 2 + (MAX_MOBILE_NUMBERS * MOBILE_NUMBER_LENGTH );

  pinMode(TRIGGER_PIN, INPUT);


  bool res;

  if(wm_nonblocking) wm.setConfigPortalBlocking(false);

  GSMMode = (EEPROM.read(GSMToggleEPROM) == 1);

  readMobileNumbersFromEEPROM();

  Serial.println(EEPROM.read(GSMToggleEPROM));
  Serial.println(ReconstructNumbers(mobileNumbers));
  int customFieldLength = 40;


  // ============================== GSM Radio Button select ==============================

  // THIS IS SHIT...... YES I've tried the way you are thinking of..... but memory problem :(

  if(GSMMode)
  {
    new (&GSMToggleParam) WiFiManagerParameter("<br/><label for='GSMToggleId'>Toggle GSM Mode</label><br></br>\
    <input type='radio' name='GSMToggleId' value='true' checked> On <br></br>\
    <input type='radio' name='GSMToggleId' value='false'> Off <br>");
  }
  else{
    new (&GSMToggleParam) WiFiManagerParameter("<br/><label for='GSMToggleId'>Toggle GSM Mode</label><br></br>\
    <input type='radio' name='GSMToggleId' value='true'> On <br></br>\
    <input type='radio' name='GSMToggleId' value='false' checked> Off <br>");
  }

  wm.addParameter(&GSMToggleParam);

  // ============================== GSM valid Number ==============================
  new (&AllowListNumbers) WiFiManagerParameter("MobileNumberAllowListId", "Enter allowed number below", ReconstructNumbers(mobileNumbers), customFieldLength,"placeholder=\"+44********** \"");
  wm.addParameter(&AllowListNumbers);


  wm.setSaveParamsCallback(saveParamCallback);

  std::vector<const char *> menu = {"wifi","info","param","sep","restart","exit"};
  wm.setMenu(menu);

  // set dark theme
  wm.setClass("invert");

 res = wm.autoConnect(AP_NAME,AP_PASSWORD); // password protected ap


  if(!res) {
      Serial.println("Failed to connect");
      //ESP.restart();
  } 
  else {
      //if you get here you have connected to the WiFi    
      Serial.println("connected...yeey :)");
  }

}

void checkButton(){
  // check for button press
  if ( digitalRead(TRIGGER_PIN) == LOW ) {
    // poor mans debounce/press-hold, code not ideal for production
    delay(50);
    if( digitalRead(TRIGGER_PIN) == LOW ){
      Serial.println("Button Pressed");
      // still holding button for 3000 ms, reset settings, code not ideaa for production
      delay(3000); // reset delay hold
      if( digitalRead(TRIGGER_PIN) == LOW ){
        Serial.println("Button Held");
        Serial.println("Erasing Config, restarting");
        wm.resetSettings();
        ESP.restart();
      }
      
      // start portal w delay
      Serial.println("Starting config portal");
      wm.setConfigPortalTimeout(120);
      
      if (!wm.startConfigPortal(AP_NAME,AP_PASSWORD)) {
        Serial.println("failed to connect or hit timeout");
        delay(3000);
        // ESP.restart();
      } else {
        //if you get here you have connected to the WiFi
        Serial.println("connected...yeey :)");
      }
    }
  }
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
  state ? EEPROM.write(GSMToggleEPROM,1) : EEPROM.write(GSMToggleEPROM,0);
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
  Serial.println("PARAM GSMToggleId = " + getParam("GSMToggleId"));
  ToggleGSM(strcmp(getParam("GSMToggleId").c_str(),"true") == 0);
  Serial.println("PARAM MobileNumberAllowListId = " + getParam("MobileNumberAllowListId"));
  SetAllowNumbers(getParam("MobileNumberAllowListId").c_str());
  
}




void GSMBufferLoop()
{
   if(Serial2.available()){
      DynamicJsonDocument jsonSerialMessage(1024);
      deserializeJson(jsonSerialMessage, Serial2.readString());
      const char* message = jsonSerialMessage["message"];
      
      if(!UtilsisStringEmpty(message))
      {
        Serial.println(UtilstoUpperCase(message)); 
        decodeMessage(UtilstoUpperCase(message));
      }
    }
}

void loop() {
  if(wm_nonblocking) wm.process();
  checkButton();
  if(GSMMode){
    GSMBufferLoop();
  }

}

#include <ArduinoJson.h>
#include <WiFiManager.h>
#include <EEPROM.h>


#define RXp2 16
#define TXp2 17

// define the number of bytes you want to access
#define EEPROM_SIZE 1
bool GSMMode = true;

WiFiManager wm;
WiFiManagerParameter GSMToggleParam; // global param ( for non blocking w params )



void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
  pinMode(LED_BUILTIN, OUTPUT);

  // initialize EEPROM with predefined size
  EEPROM.begin(EEPROM_SIZE);

  bool res;


  if(EEPROM.read(0) == 1)
  {
    GSMMode = true;
  }else{
    GSMMode = false;
  }


  // add a custom input field
  int customFieldLength = 40;
  char custom_radio_str[300];

  
  sprintf(custom_radio_str, "<br/><label for='GSMToggle'>Toggle GSM Mode</label><br></br><input type='radio' name='customfieldid' value='true' %s> On<br><input type='radio' name='customfieldid' value='false' %s> Off",
        (EEPROM.read(0) == 1 ? "checked" : ""),
        (EEPROM.read(0) == 0 ? "checked" : "")
  );

  new (&GSMToggleParam) WiFiManagerParameter(custom_radio_str); // custom html input
  
  wm.addParameter(&GSMToggleParam);
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


void saveParamCallback(){
  Serial.println("[CALLBACK] saveParamCallback fired");
  Serial.println("PARAM customfieldid = " + getParam("GSMToggle"));
  if(strcmp(getParam("customfieldid").c_str(),"true") == 0){
    EEPROM.write(0,1);
  }
  else{
     EEPROM.write(0,0);
  }
  EEPROM.commit();
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

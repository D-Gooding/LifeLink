#include <ArduinoJson.h>
#include <WiFiManager.h>
#include <EEPROM.h>
#include <PubSubClient.h> 
#include "Webserver_Utils.h"
#include "General_Utils.h"
#include "Temperature_Sensor_Utils.h"
#include <DHT.h>

// ============================ Define Temperature sensor ============================
#define DHT_SENSOR_PIN  21 // ESP32 pin GPIO21 connected to DHT22 sensor
#define DHT_SENSOR_TYPE DHT22

DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);
RollingAverage* TemperatureRollingAverage;


//============================ Serial Coms for Arduino->GSM board ============================
#define RXp2 16
#define TXp2 17




//============================ WIFI MANAGER ============================
#define TRIGGER_PIN 0
#define AP_NAME "LifeLinkAP"
#define AP_PASSWORD "password"

WiFiManager wm;
WiFiManagerParameter GSMToggleParam; // global param ( for non blocking w params )
WiFiManagerParameter AllowListNumbers;
bool wm_nonblocking = false; // change to true to use non blocking

bool GSMMode = true;
int GSMToggleEPROM = 0;

bool isWifiConnected = false;

//============================ MQTT Defualts ============================
#define MQTT_BROKER "test.mosquitto.org" 
#define MQTT_PORT (1883) 
#define MQTT_PUBLIC_TOPIC "uok/iot/dmag2/LowRoomTemp"
#define MQTT_PUBLIC_Receive_TvCommand_TOPIC "uok/iot/dmag2/TvCommand"
WiFiClient wifiClient; 
PubSubClient client(wifiClient); 




char mobileNumbers[MAX_MOBILE_NUMBERS][MOBILE_NUMBER_LENGTH];

//GSM Outbox buffer
const size_t MAX_NUMBER_MESSAGES = 10;
std::deque<StaticJsonDocument<200>*> OutboxMessages;


void MQTTcallback(char* topic, byte* payload, unsigned int length) {
  // Handle the received message here
  Serial.println("Message arrived in topic: " + String(topic));
  Serial.println("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void DefaultMQTTSubs()
{
  client.subscribe(MQTT_PUBLIC_Receive_TvCommand_TOPIC);
  client.setCallback(MQTTcallback);
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
  pinMode(LED_BUILTIN, OUTPUT);
  dht_sensor.begin();
  TemperatureRollingAverage = new RollingAverage();

  // initialize EEPROM with predefined size
  EEPROM.begin(20 + (MAX_MOBILE_NUMBERS * MOBILE_NUMBER_LENGTH ));
  GSMToggleEPROM = 2 + (MAX_MOBILE_NUMBERS * MOBILE_NUMBER_LENGTH );

  pinMode(TRIGGER_PIN, INPUT);



  if(wm_nonblocking) wm.setConfigPortalBlocking(false);

  GSMMode = (EEPROM.read(GSMToggleEPROM) == 1);

  readMobileNumbersFromEEPROM();

  Serial.println(EEPROM.read(GSMToggleEPROM));
  Serial.println(ReconstructNumbers(mobileNumbers));
  int customFieldLength = 40;


  client.setServer(MQTT_BROKER, MQTT_PORT); //set broker settings



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

  isWifiConnected = wm.autoConnect(AP_NAME,AP_PASSWORD); // password protected ap


  if(!isWifiConnected) {
      Serial.println("Failed to connect");
      //ESP.restart();
  } 
  else {
      //if you get here you have connected to the WiFi    
      Serial.println("connected...yeey :)");
      DefaultMQTTSubs();

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
      if (!wm.startConfigPortal(AP_NAME,AP_PASSWORD)) {
        Serial.println("failed to connect or hit timeout");
        delay(3000);
        isWifiConnected = false;
      } else {
        //if you get here you have connected to the WiFi
        Serial.println("connected...yeey :)");
        DefaultMQTTSubs();
        isWifiConnected = true;

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
  Serial.print("GSMENABLED");
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

/*
* Samples the current temperature and sees if we need to 
* Alert a career of a low temperature
*/
void CheckRoomTemperature()
{
  unsigned long currentMillis = millis();
  float currentTemperature = dht_sensor.readTemperature();
  if(shouldSendLowTempAlert(currentMillis,currentTemperature,TemperatureRollingAverage))
  {
    char message[50];
    snprintf(message, sizeof(message), "LOW TEMPERATURE!!! %.2f C", TemperatureRollingAverage->getAverage());
    Serial.println(message);
    SendToOutbox(message);
  }
}


/*
* For all of the saved numbers add to the send buffer all the messages.
*/
void SendToOutbox(char* message)
{
  for (int i = 0; i < MAX_MOBILE_NUMBERS; i++) {
    const char* currentMobileNumber = mobileNumbers[i];

    // Check if the current mobile number is a valid number
    if (isMobileNumber(currentMobileNumber) && strlen(currentMobileNumber) > 0) {
      Serial.println(currentMobileNumber);
      StaticJsonDocument<200>* jsonSerialMessage = new StaticJsonDocument<200>();
      (*jsonSerialMessage)["p"] = currentMobileNumber;
      (*jsonSerialMessage)["m"] = message;
      OutboxMessages.push_back(jsonSerialMessage);
    }
  }
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

// For all saved numbers send the messages in the buffer
void GSMOutboxLoop()
{
   if (!OutboxMessages.empty()) {
    Serial.println("Message awaiting to be sent");
    StaticJsonDocument<200>* message = OutboxMessages.front();
    serializeJson(*message, Serial2);
    //delay added to give it time to send
    delay(20);
    OutboxMessages.pop_front();
    delete message;
  }


}

void loop() {
  if(wm_nonblocking) wm.process();
  checkButton();
  CheckRoomTemperature();
  if(GSMMode){
    GSMBufferLoop();
    GSMOutboxLoop();
  }

}

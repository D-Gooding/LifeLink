#include <string.h>
#include <SoftwareSerial.h> //allows connection between the two boards
#include <ArduinoJson.h>


char buffer[256]; //stops overflow of arduino

SoftwareSerial GSM(7, 8); //RX and TX

bool USB_DEBUG_MODE = false;


//Added debugging options on the serial outputs 
void serialPrintString(const __FlashStringHelper c[])
{
  if (USB_DEBUG_MODE)
  {
    Serial.println(c);
  }
}
void serialPrintString(const char c[])
{
  if(USB_DEBUG_MODE)
  {
    Serial.println(c);
  }
}

void checkCSQAtCodeForStrength(const char str[]) {
    char *csqPtr = strstr(str, "+CSQ:");
    if (csqPtr == NULL) {
      return;
      }

    csqPtr += 6; // Move the pointer to the character after '+CSQ:'
    for (; *csqPtr != '\0'; csqPtr++) {
        if (*csqPtr != '0' && *csqPtr != ',' && *csqPtr != ' ') {
          return;
        }
    }

    Serial.println("GSM-CONNECTED");
}


void setup() {
  
  Serial.begin(9600);
  GSM.begin(9600); //software interface
  //ESPSerial.begin(19200);
  SIM900power();

  //ESPSerial.print("HEEYY");
  serialPrintString(F("waiting for device connection"));
  for(int i = 0; i<10; i++){
    serialPrintString(F("."));
    delay(1000);
  }
  serialPrintString(F(""));

  //send Config commands to set the board up

  //query NET Provider
  serialPrintString(F("Connecting To NET"));
  GSM.print("AT+COPS?\r");
  readGSMBuffer();
  serialPrintString(buffer);
  
  //SET SMS mode to text
  serialPrintString(F("Setting SMS message to text"));
  GSM.print("AT+CMGF=1\r");
  readGSMBuffer();
  serialPrintString(buffer);

  //Send SMS message to Arduino via serial
  serialPrintString(F("Forwarding SMS commands to serial"));
  GSM.print("AT+CNMI=2,2,0,0,0\r");
  readGSMBuffer();
  serialPrintString(buffer);

  //get Number
  serialPrintString(F("Get Signal Strength"));
  GSM.print("AT+CSQ\r");
  readGSMBuffer();
  checkCSQAtCodeForStrength(buffer);

  serialPrintString(F("Attempt to get number"));
  GSM.print("AT+CNUM\r");
  readGSMBuffer();
  serialPrintString(buffer);

  // Caller ID setup
  serialPrintString(F("Enabling Caller ID on incoming calls"));
  GSM.print("AT+CLIP=1\r");
  readGSMBuffer();
  serialPrintString(buffer);

  //done
  serialPrintString("Done");
  sendSMS("+447927401195","GSM Online");
  
}

/*
 * This reads the GSM module buffer
 */

void readGSMBuffer(){
  //clear the buffer
  memset(buffer, '\0', sizeof(buffer));
  //The max time for a messsage to be sent
  unsigned int timeout = 5000;
  //the max time in ms for two characters in a message before timeout
  unsigned int chartimeout = 1000;
  //num character to be read
  int i = 0;
  //time of first character of message
  unsigned long timeStart = millis();
  //time at which most recent character of message was recived
  unsigned long timeLastCharReceived = 0;

  while(true){
    while(GSM.available()){
      //read next
      char c = GSM.read();
      timeLastCharReceived = millis();
      buffer[i++] = c;
      if(i >= sizeof(buffer)){
        break;
      }
    }
    //if max char read then stop
    if(i >= sizeof(buffer)){
      break;
    }
    //if the total time taken to receive this messsage exceed the time out,STOP
    if((unsigned long)(millis() - timeStart) > timeout){
      break;
    }
    //if the time since the last char was received exceed the time out Stop
    if((timeLastCharReceived > 0)  &&  ((unsigned long)(millis() - timeLastCharReceived) > chartimeout)) {
      break;
    }

    
  }
}

/*
 * If a text is recived, turn the data into a Readable string
 * This code was sourced from this youtube video i can no longer find :(
 */
bool receiveSMS(char *message, char *phone, char *datetime){
  //if SMS has been received buffer will contain data in the format
  // +CMT : "+44***********","","date","time"

  //counter
  int i = 0;
  char *p,*p2,*s;
  //search for "+CMT:"
  if((s = strstr(buffer, "+CMT:")) != NULL){
    //get phone num string
    p = strstr(s,"\"");
    //p2 is the next char which is the start of the phone number
    p2 = p + 1;
    p = strstr((char *)(p2), "\"");
    //if phone number found, enclosed with ""
    if(p != NULL){
      i = 0;
      while (p2 < p){
        phone[i++] = *(p2++);
      }
      phone[i] = '\0';
      
    }
    //get date time string
    p = strstr((char *)(p2),",");
    p2 = p + 1;
    p = strstr((char *)(p2),",");
    p2 = p + 2; //we are in the first date time char
    p = strstr((char *)(p2), "\"");
    if(p != NULL){
      i = 0;
      while(p2 < p){
        datetime[i++] = *(p2++);
      }
      datetime[i] = '\0';
    }
    //Extract messsage
    if((s = strstr(s,"\r\n")) != NULL){
      i = 0;
      p = s + 2;
      //sms messages are 160 max
      while((*p != '\r')&&(i  < 159)){
        message[i++] = *(p++);
      }
      message[i] = '\0';
    }
    return true;
    
  }
  //if no CMT was found 
  return false;

  
}

/*
 * Receives a Call
 */

bool receiveCall(char *phone){
  //gets data from buffer and outputs
  int i = 0;
  //tokenise
  char *p, *p2, *s;
  if((s= strstr(buffer, "RING")) != NULL){
    //gets number
    p = strstr(s,"\"");
    p2 = p + 1;
    p = strstr((char *)(p2), "\"");
    if(p !=NULL){
      i = 0;
      while (p2 < p){
        phone[i++] = *(p2++);
      }
      phone[i] = '\0';
    }
    return true;
  }
  return false;

  
}

/*
 * Used to power up the GSM board along with the Arduino 
 */
void SIM900power()
{
  pinMode(9, OUTPUT); 
  digitalWrite(9,LOW);
  delay(1000);
  digitalWrite(9,HIGH);
  delay(2000);
  digitalWrite(9,LOW);
  delay(3000);
}

/*
 * Sends a message to a list of allowed numbers
 */

void sendSMS(const char* phoneNumber,const char* message)
{

  char atCommand[50];
  snprintf(atCommand, sizeof(atCommand), "AT+CMGS=\"%s\"\r\n", phoneNumber);
  GSM.print(atCommand);

  delay(3000);
  GSM.print(message);
  GSM.print((char)26);
  GSM.print("\r\n");
}


/*
 * Checks if any new messages are needed to be sent
 */
void checkOutbox()
{
  if (Serial.available()) {
    Serial.println("Reading");

    // Read the incoming data until a newline character is encountered
    String jsonString = Serial.readStringUntil('\n');

    serialPrintString(jsonString.c_str());

    // Create a JSON document
    StaticJsonDocument<200> jsonSerialMessage;

    DeserializationError error = deserializeJson(jsonSerialMessage, jsonString);

    // Check for deserialization errors
    if (error) {
      serialPrintString(F("Deserialization failed: "));
      serialPrintString(error.c_str());
    } else {
      const char* phone = jsonSerialMessage["p"];
      const char* message = jsonSerialMessage["m"];

      serialPrintString("Phone: ");
      serialPrintString(phone);
      serialPrintString("Message: ");
      serialPrintString(message);
      
      sendSMS(phone, message);
    }
  }
}

void loop() {
  readGSMBuffer();
  //if buffer not empty
  uint32_t keyCode = 0;
  if(buffer[0] != '\0'){
    serialPrintString(buffer);
    //these variables will be populated by the data given

    char phone[16];
    char datetime[24];
    char message [160];

    //string commands get turned into actions
    if(receiveSMS(message, phone, datetime)){
      
      StaticJsonDocument<200> jsonSerialMessage;
      jsonSerialMessage["phoneNumber"] = phone;
      jsonSerialMessage["dateTime"] = datetime;
      jsonSerialMessage["message"] = message;

      serializeJson(jsonSerialMessage, Serial);
      Serial.println();
    }
    
  }
  checkOutbox();

}
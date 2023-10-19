#include <string.h>

//

#include <SoftwareSerial.h> //allows connection between the two boards

//sms is 160 max

char buffer[256]; //stops overflow of arduino

SoftwareSerial GSM(7, 8); //RX and TX 

bool USB_DEBUG_MODE = false;


//Added debugging options on the serial outputs 
void serialPrintString(const __FlashStringHelper c[], bool NewLine = true)
{
  USB_DEBUG_MODE ? true : Serial.print(c);
  NewLine ? true : Serial.println("");
}

void serialPrintString(const char c[], bool NewLine = true)
{
  USB_DEBUG_MODE ? true : Serial.print(c);
  NewLine ? true : Serial.println("");
}


void setup() {
  
  Serial.begin(9600);
  GSM.begin(9600); //software interface
  SIM900power();
  Serial.print(F("waiting for device connection"));
  for(int i = 0; i<10; i++){
    serialPrintString(F("."),false);
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
  //EDIT this later to pass a connected check
  serialPrintString(buffer);

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
  sendSMS();
  
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
 * This is a check alive text that can be done to check if the system is online.
 */

void sendSMS()
{
  GSM.print("AT+CMGS=\"+447927401195\"\r\n");
  delay(3000);
  GSM.print("GSM System Online");
  GSM.print((char)26);
  GSM.print("\r\n");
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
      serialPrintString(F("Incoming SMS from number: "));
      char* ESP32Message = strcat(strcat(strcat(phone,"|"),strcat(message,"|")),datetime); 
      Serial.println(ESP32Message);       
    }
    
  }

}
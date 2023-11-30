#include <IRremote.hpp>
#include <map>
#include <String>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>


#define IR_RECEIVE_PIN 15       // PA3 - on Digispark board labeled as pin 9
#define MARK_EXCESS_MICROS 20  // Adapt it to your IR receiver module. 20 is recommended for the cheap VS1838 modules.

#define HARDWARE_TYPE MD_MAX72XX::GENERIC_HW
#define MAX_DEVICES 1

#define CLK_PIN   18
#define DATA_PIN  23
#define CS_PIN    5

MD_Parola display = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);


std::string DisplayBuffer = "";

//============================ IR ============================
const std::map<byte, std::string> TV_IR_COMMANDS = {
    {0x19, "0"},
    {0x10, "1"},
    {0x11, "2"},
    {0x12, "3"},
    {0x13, "4"},
    {0x14, "5"},
    {0x15, "6"},
    {0x16, "7"},
    {0x17, "8"},
    {0x18, "9"},
    {0x3D, "ON"},
    {0x20, "VOL+"},
    {0x21, "VOL-"}
};


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  display.begin();
  display.setScrollSpacing(7);
  display.setSpeed(100);
  Serial.begin(115200);
  delay(4000);  // To be able to connect Serial monitor after reset or power up and before first print out. Do not wait for an attached Serial Monitor!

  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
}


bool isIRCommandValid(byte receivedCommand) {
    return TV_IR_COMMANDS.find(receivedCommand) != TV_IR_COMMANDS.end();
}


void loop() {
  if(IrReceiver.decode()){
     byte receivedAddress = IrReceiver.decodedIRData.address;
        byte receivedCommand = IrReceiver.decodedIRData.command;

        Serial.println();
        Serial.println();
        Serial.println(receivedAddress, HEX);
        Serial.println(receivedCommand, HEX);

        if (isIRCommandValid(receivedCommand)) {
            Serial.println("Valid IR command received!");
            DisplayBuffer = TV_IR_COMMANDS.at(receivedCommand) + "   ";
            // Do something when a valid command is received
        } else {
            Serial.println("Invalid IR command received!");
            // Do something when an invalid command is received
        }
        IrReceiver.resume();
  }

  if (display.displayAnimate())
    display.displayText(DisplayBuffer.c_str(), PA_CENTER, display.getSpeed(), 500, PA_SCROLL_LEFT);
}

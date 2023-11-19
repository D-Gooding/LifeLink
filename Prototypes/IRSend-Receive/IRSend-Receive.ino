#include <Arduino.h>
#include <IRremote.hpp>

const int SEND_PIN = 4;  // Pin connected to the IR emitter
const int RECV_PIN = 15; // Pin connected to the IR receiver

uint16_t sAddress = 0x0102;
uint8_t sCommand = 0x34;
uint8_t sRepeats = 1;

decode_results results;

void setup() {
  IrReceiver.begin(RECV_PIN, true);
  IrSender.begin(SEND_PIN);

  IrSender.enableIROut(38); // Call it with 38 kHz to initialize the values printed below
  IrReceiver.enableIRIn(); // Start the IR receiver
  Serial.begin(115200);
}


void send_ir_data() {
    Serial.print(F("Sending: 0x"));
    Serial.print(sAddress, HEX);
    Serial.print(sCommand, HEX);
    Serial.println(sRepeats, HEX);
    Serial.flush(); // To avoid disturbing the software PWM generation by serial output interrupts

    // clip repeats at 4
    if (sRepeats > 4) {
        sRepeats = 4;
    }
    // Results for the first loop to: Protocol=NEC Address=0x102 Command=0x34 Raw-Data=0xCB340102 (32 bits)
    IrSender.sendNEC(sAddress, sCommand, sRepeats);
}

void receive_ir_data() {
    if (IrReceiver.decode()) {
        Serial.print(F("Decoded protocol: "));
        Serial.print(getProtocolString(IrReceiver.decodedIRData.protocol));
        Serial.print(F(", decoded raw data: "));
#if (__INT_WIDTH__ < 32)
        Serial.print(IrReceiver.decodedIRData.decodedRawData, HEX);
#else
        PrintULL::print(&Serial, IrReceiver.decodedIRData.decodedRawData, HEX);
#endif
        Serial.print(F(", decoded address: "));
        Serial.print(IrReceiver.decodedIRData.address, HEX);
        Serial.print(F(", decoded command: "));
        Serial.println(IrReceiver.decodedIRData.command, HEX);
        IrReceiver.resume();
    }
}




void loop() {
  send_ir_data();
  IrReceiver.restartAfterSend(); // Is a NOP if sending does not require a timer.
  delay(5000);
  receive_ir_data();

}

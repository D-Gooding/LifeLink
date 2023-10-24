#include <IRremote.h>

IRsend IRSender;
#define IR_SEND_PIN 4

byte channelHexCode[] = {0x910,0x10,0x810,0x410,0xC10,0x210,0xA10,0x610,0xE10,0x110};

void setup() {
  // put your setup code here, to run once:
  IrSender.begin(IR_SEND_PIN);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < 4; i++) {
              IRSender.sendSony(channelHexCode[1], 12);
              delay(30);
            }
          IRSender.sendSony(0xFFFFFFF, 12);  
}

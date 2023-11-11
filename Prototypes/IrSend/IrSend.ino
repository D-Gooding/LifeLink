#include <IRremote.h>

IRsend IRSender;
#define IR_SEND_PIN 4

byte channelHexCode[] = {0x910,0x10,0x810,0x410,0xC10,0x210,0xA10,0x610,0xE10,0x110};

#define PanasonicAddress      0x4004     // Panasonic address (Pre data) 
#define PanasonicPower        0x100BCBD  // Panasonic Power button

void setup() {
  // put your setup code here, to run once:
  IrSender.begin(IR_SEND_PIN);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < 4; i++) {
              IRSender.sendPanasonic(PanasonicAddress, PanasonicPower,4);
              delay(30);
            }
          IRSender.sendPanasonic(0xFFFFFFF, 12);  
}

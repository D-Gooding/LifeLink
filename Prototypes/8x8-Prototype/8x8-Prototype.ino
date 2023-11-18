#include <Adafruit_GFX.h>

#include <Max72xxPanel.h>  // max7219 library

Max72xxPanel matrix = Max72xxPanel(5, 1, 1);  // Adjust parameters based on your hardware setup

const uint8_t SMILEY_FACE[] = {
  0b00111100,
  0b01000010,
  0b10100101,
  0b10000001,
  0b10100101,
  0b10011001,
  0b01000010,
  0b00111100
};


const uint8_t WINKY_FACE_3[] = {
  0b00111100,
  0b01000010,
  0b10100101,
  0b10000001,
  0b10100101,
  0b10011001,
  0b01000010,
  0b00111100
};

void drawImage(const uint8_t* image, int size) {
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < 8; j++) {
      if ((image[i] >> j) & 1) {
        matrix.drawPixel(j, i, 1);
      }
    }
  }
}
void setUpMatrix() {
  matrix.setIntensity(10);
  matrix.setRotation(0);
  matrix.fillScreen(0);
  matrix.write();
}

void setup() {
  Serial.begin(115200);
  setUpMatrix();

}

void loop() {
  matrix.fillScreen(0);
  drawImage(SMILEY_FACE, sizeof(SMILEY_FACE) / sizeof(SMILEY_FACE[0]));
  Serial.println("Happy");

  matrix.write();
  delay(1000);

  matrix.fillScreen(0);

  drawImage(WINKY_FACE_3, sizeof(SMILEY_FACE) / sizeof(SMILEY_FACE[0]));
  matrix.write();
  delay(5000);
}

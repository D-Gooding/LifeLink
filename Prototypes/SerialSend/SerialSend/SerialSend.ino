#include <SoftwareSerial.h> //allows connection between the two boards


// this sample code provided by www.programmingboss.com


void setup() {
  Serial.begin(9600);
}
void loop() {
  Serial.println("Hello Boss");
  delay(1500);
}

//11 12
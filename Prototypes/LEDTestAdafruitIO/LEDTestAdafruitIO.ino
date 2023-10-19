#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "WiFi.h"



#define WIFI_SSID "No you"
#define WIFI_PASS "helloworld"

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME "dgooding"
#define AIO_KEY "mydata"
#define LED_PIN 2 // You can change this to match your board's LED pin

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);

  // Connect to Adafruit IO
  io.connect();

  // Define a feed to control the LED
  AdafruitIO_Feed *led_feed = io.feed("led");
}

void loop() {
  io.run();

  AdafruitIO_Feed *led_feed = ("led");

  if (led_feed->readLast() == "ON") {
    digitalWrite(LED_PIN, HIGH); // Turn on the LED
  } else if (led_feed->readLast() == "OFF") {
    digitalWrite(LED_PIN, LOW); // Turn off the LED
  }
}


#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// Wi-Fi credentials
const char* ssid = "No you";
const char* password = "helloworld";

// Adafruit IO credentials
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "dgooding"
#define AIO_KEY         "mydata"

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");

    // Connect to Adafruit IO
    mqtt.connect();
}

void loop() {
    // Your code here
    
    // Example of publishing data to Adafruit IO feed
    Adafruit_MQTT_Publish feed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/Feeds/mydata");
    if (feed.publish("Hello, Adafruit IO!")) {
        Serial.println("Message sent to Adafruit IO.");
    }
    delay(5000); // Publish every 5 seconds
}

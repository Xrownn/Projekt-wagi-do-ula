#include <WiFi.h>
#include "ThingSpeak.h"

const char* ssid = "Network Name";
const char* password = "Password";

unsigned long ChannelNumber = ; // ID Number
const char* APIKey = "APIKey";

WiFiClient client;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  ThingSpeak.begin(client);
}

void loop() {
  int test = random(20, 50);

  ThingSpeak.setField(1, test);
  
  int httpCode = ThingSpeak.writeFields(ChannelNumber, APIKey);

  delay(20000);
}

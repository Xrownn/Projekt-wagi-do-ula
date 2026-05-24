#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>
#include "ThingSpeak.h"

// LoRa pins
#define SS 5
#define RST 22
#define DIO0 21

// Wi-Fi data
const char* ssid = "Network Name";
const char* password = "Password";

// ThingSpeak config
unsigned long ChannelNumber = 0; //ID
const char* APIKey = "APIKey";

WiFiClient client;

void setup() {
  Serial.begin(115200);

// initialize LoRa
  SPI.begin();
  LoRa.setPins(SS,RST,DIO0);

  while (!LoRa.begin(868E6)) {
    Serial.println(".");
    delay(500);
  }
  // setting transmission parameters
  LoRa.setSyncWord(0x12);
  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(8);
  LoRa.enableCrc();

// initialize WiFi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

// initialize ThingSpeak
  ThingSpeak.begin(client);
}

void loop() {
  // receiving data
  if (LoRa.parsePacket()) {
    String weight_str = "";
    while (LoRa.available()) {
      char receive = (char)LoRa.read();
      weight_str += receive;
    }
  //converting and sending data
  float weight = weight_str.toFloat();
  ThingSpeak.setField(1, weight);
  int httpCode = ThingSpeak.writeFields(ChannelNumber, APIKey);
  }
}

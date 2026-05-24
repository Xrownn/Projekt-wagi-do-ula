#include <SPI.h>
#include <LoRa.h>

#define SS 5
#define RST 22
#define DIO0 21
#define LED 2

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  SPI.begin();
  LoRa.setPins(SS,RST,DIO0);

  while (!LoRa.begin(868E6)) {
    Serial.println(".");
    delay(500);
  }

  LoRa.setSyncWord(0x12);
  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(8);
  LoRa.enableCrc();
  LoRa.setTxPower(14);
}

void loop() {
  static int waga = 0;
  LoRa.beginPacket();
  LoRa.print("Waga wynosi=");
  LoRa.print(++waga) ;
  LoRa.endPacket();
  digitalWrite(2, HIGH);
  delay(500);
  digitalWrite(2, LOW);
  Serial.print("wyslano");
  delay(10000);

}

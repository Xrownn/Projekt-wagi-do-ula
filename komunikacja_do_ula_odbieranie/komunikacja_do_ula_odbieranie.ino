#include <SPI.h>
#include <LoRa.h>

#define SS 5
#define RST 22
#define DIO0 21

void setup() {
  Serial.begin(115200);
  SPI.begin(18, 19, 23);
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
}

void loop() {
  if (LoRa.parsePacket()) {
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }
    Serial.printf("(%d)\n", LoRa.packetRssi());
  }

}

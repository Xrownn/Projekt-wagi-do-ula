#include "Adafruit_HX711.h"
#include <SPI.h>
#include <LoRa.h>

// hx711 pins
#define data_pin 16
#define clock_pin 17

Adafruit_HX711 hx711(data_pin,clock_pin);

// LoRa pins
#define SS 5
#define RST 22
#define DIO0 21
#define LED 2

// millis parameters 
const unsigned long interval = 10000;
unsigned long last_measurement_time = 0;

void setup() {
  Serial.begin(115200);

// initialize hx711
  hx711.begin();
  // taring
  long suma = 0;
  for (int i = 0; i < 10; i++) {
    suma += hx711.readChannelBlocking(CHAN_A_GAIN_128);
  }
  hx711.tareA(suma / 10);

// initialize LoRa
  SPI.begin();
  LoRa.setPins(SS,RST,DIO0);

  while (!LoRa.begin(868E6)) {
    delay(500);
  }
  // setting transmission parameters
  LoRa.setSyncWord(0x12);
  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(8);
  LoRa.enableCrc();
  LoRa.setTxPower(14);
}

void quicksort(long in_array[],int start, int stop){
    int i = start;
    int j = stop;
    long pivot = in_array[(start+stop)/2];
    while (i<=j){
      while (in_array[i]<pivot) i++;
      while (in_array[j]>pivot) j--;

      if (i<=j){
        long temp = in_array[i];
        in_array[i] =  in_array[j];
        in_array[j] = temp;
        i++;
        j--;
      }
    }
    if (start < j) quicksort(in_array, start, j);
    if (i < stop) quicksort(in_array, i, stop);
}
    
long get_average_measurement(long measurements[]){
    quicksort(measurements,0,14);
    long sum = 0;
    for(int i=3;i<=11;i++) sum+= measurements[i];
    return sum/9;
}


void loop() {
  unsigned long now = millis();

  if(now - last_measurement_time >= interval){
    last_measurement_time = now;
  // read weight 
    long measurements[15];
    for (int i = 0; i < 15; i++){
      measurements[i]= hx711.readChannelBlocking(CHAN_A_GAIN_128);
    }
    long average_measurement = get_average_measurement(measurements);
    float weight = average_measurement*(457.0/10432.0);

  // transmit weight by LoRa
    LoRa.beginPacket();
    LoRa.print("Weight = ");
    LoRa.print(weight);
    LoRa.endPacket();
  }

}

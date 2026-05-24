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
// Led for testing
// #define LED 2

// deep slip parameters 
const unsigned long Sleep_time = 3600*1000000; // value before '*' in seconds
RTC_DATA_ATTR int32_t tare_value = 0;
RTC_DATA_ATTR bool is_tared = false;

void quicksort(int32_t in_array[],int start, int stop){
    int i = start;
    int j = stop;
    int32_t pivot = in_array[(start+stop)/2];
    while (i<=j){
      while (in_array[i]<pivot) i++;
      while (in_array[j]>pivot) j--;

      if (i<=j){
        int32_t temp = in_array[i];
        in_array[i] =  in_array[j];
        in_array[j] = temp;
        i++;
        j--;
      }
    }
    if (start < j) quicksort(in_array, start, j);
    if (i < stop) quicksort(in_array, i, stop);
}
    
int32_t get_average_measurement(int32_t measurements[]){
    quicksort(measurements,0,14);
    int32_t sum = 0;
    for(int i=3;i<=11;i++) sum+= measurements[i];
    return sum/9;
}

void setup() {
  Serial.begin(115200);

// initialize hx711
  hx711.begin();
  // taring
  if(!is_tared){
    for (int i = 0; i < 10; i++) {
      tare_value += hx711.readChannelBlocking(CHAN_A_GAIN_128);
    }
    tare_value = tare_value/10;
    is_tared = true;
  }

  hx711.tareA(tare_value);

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

  // digitalWrite(LED, HIGH);
  
  // read weight 
  int32_t measurements[15];
  for (int i = 0; i < 15; i++){
    measurements[i]= hx711.readChannelBlocking(CHAN_A_GAIN_128);
  }
  int32_t average_measurement = get_average_measurement(measurements);
  float weight = average_measurement*(457.0/10432.0);

  // transmit weight by LoRa
  LoRa.beginPacket();
  LoRa.print(weight);
  LoRa.endPacket();

  // digitalWrite(LED, LOW);

  // sleep
  LoRa.end();
  hx711.powerDown(true);
  esp_sleep_enable_timer_wakeup(Sleep_time);
  esp_deep_sleep_start();

}

void loop(){

}

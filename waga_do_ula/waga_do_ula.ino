#include "Adafruit_HX711.h"

#define data_pin 19
#define clock_pin 18

Adafruit_HX711 hx711(data_pin,clock_pin);

void setup() {
  Serial.begin(115200);
  hx711.begin();
  long suma = 0;
  for (int i = 0; i < 10; i++) {
    suma += hx711.readChannelBlocking(CHAN_A_GAIN_128);
  }
  hx711.tareA(suma / 10);
}

void _quicksort(long in_array[],int start, int stop){
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
    if (start < j) _quicksort(in_array, start, j);
    if (i < stop) _quicksort(in_array, i, stop);
}
    
long sredni_wynik(long pomiary[]){
    _quicksort(pomiary,0,14);
    long suma = 0;
    for(int i=3;i<=11;i++) suma+= pomiary[i];
    return suma/9;
}


void loop() {
  long pomiar[15];
  for (int i = 0; i < 15; i++){
    pomiar[i]= hx711.readChannelBlocking(CHAN_A_GAIN_128);
  }
  long pomiar_sre = sredni_wynik(pomiar);
  float waga = pomiar_sre*(457.0/10432.0);
  Serial.print("Waga wynosi: ");
  Serial.println(waga);
  delay(1000);
}

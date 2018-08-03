#include <elapsedMillis.h>

elapsedMicros usec = 0;
int dac_pin = A0;

void setup() {
  Serial.begin(115200);
  analogWriteResolution(10); // ATSAMD21 has 10 bit DAC

  // With Feather M0 Vcc measured 3.5V avg (3.66V max)
  // 1023 gives 3.49V avg (3.55V max)
  // 901 gives 
  // 900 gives 3.06V avg (3.11V max), MESH 2.85 for sure. 2.9 rarely
  // 850 gives 2.91V avg (3.01V max) 
  // 100 gives 0.53V avg (0.59V max)
  // 0 gives 0.215V avg (0.378V max)
  //analogWrite(dac_pin, 100);
}

void loop() {

  // with Feather M0 Vcc measured 3.5V avg (3.66V max)
  // use 20 to 916 as analogWrite range
  // MESH 2.95 --> 916 or up (915 rarely 916 relyably) <-- use 916 as max value
  // MESH 2.90 --> 901 or up
  // MESH 2.85 --> 886 or up
  // MESH 2.80 --> 868 or up
  // MESH 0.20 --> 57 or up
  // MESH 0.15 --> 43 or up
  // MESH 0.10 --> 29 or up
  // MESH 0.05 --> 14 or up
  analogWrite(dac_pin, 15);
  delay(100);  // needs debounce time of min 30ms. 50ms is good. 100ms to be very safe
  analogWrite(dac_pin, 0);
  delay(1500);

//  for (int i = 0; i<501; i++){
//    analogWrite(dac_pin, i);
//    Serial.print(millis());
//    Serial.print(": ");
//    Serial.println(i);
//    while (usec < 1000); // wait
//    usec = usec - 1000;
//  }
}

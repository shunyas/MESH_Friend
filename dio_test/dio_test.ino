#include <elapsedMillis.h>

elapsedMicros usec = 0;

// Pins for MESH. in/out naming are from MESH's point of view
int ain = A0;
int di1 = 10;
int di2 = 6;
int di3 = 5;
int do1 = 13; // connected to Feather's red LED
int do2 = 12;
int do3 = 11;
int pwmo = A5;
int dis[] = {di1, di2, di3};

int val1, val2, val3 = 0;
unsigned long duration;

void setup() {
  Serial.begin(115200);
  analogWriteResolution(10); // ATSAMD21 has 10 bit DAC
  for (int i=0; i<3; i++){
    pin = dis[i];
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);    
  }

  pinMode(do1, INPUT);
  pinMode(do2, INPUT);
  pinMode(do3, INPUT);
  pinMode(pwmo, INPUT);
}

void loop() {
  // test_analog_in();
  // test_din();
  // test_dout();
  // test_pwmo();
}

void test_pwmo(void){
  // PWM output is at 125Hz
  // 1 to 99 values in MESH corresponds well with duty cycle percentage measurement
  // 0 would be continuous 0.1V and 100 be continuous 2.4V
  // pulseIn for 0 and 100 return 0 after timeout period
  // 125Hz is 8ms/cycle
  // 1% duty cycle = 80us 63
  // Duty cyle | on time | pulseIn
  // 1% 80  63
  // 2% 160 157
  // 50% 4000 3974
  // 99% 7920 7888
  duration = pulseIn(pwmo, HIGH);
  Serial.println(duration);
  delay(1000);
}

void test_dout(void){
  int val = digitalRead(do1);
  // Serial.println(val);
  if (val != val1) {
    Serial.println(val);
    val1 = val;
  }
  delay(10);  
}

void test_din(void){
  for (int i=0; i<3; i++){
    int pin = dis[i];
    digitalWrite(pin, LOW);
    Serial.println("LOW!");
    delay(100);
    digitalWrite(pin, HIGH);
    delay(1000);
  }
}

void test_analog_in(void){
  // With Feather M0 Vcc measured 3.5V avg (3.66V max)
  // 1023 gives 3.49V avg (3.55V max)
  // 901 gives 
  // 900 gives 3.06V avg (3.11V max), MESH 2.85 for sure. 2.9 rarely
  // 850 gives 2.91V avg (3.01V max) 
  // 100 gives 0.53V avg (0.59V max)
  // 0 gives 0.215V avg (0.378V max)
  //analogWrite(ain, 100);

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
  analogWrite(ain, 15);
  delay(100);  // needs debounce time of min 30ms. 50ms is good. 100ms to be very safe
  analogWrite(ain, 0);
  delay(1500);

//  for (int i = 0; i<501; i++){
//    analogWrite(ain, i);
//    Serial.print(millis());
//    Serial.print(": ");
//    Serial.println(i);
//    while (usec < 1000); // wait
//    usec = usec - 1000;
//  }
  }


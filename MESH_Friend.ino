#include <elapsedMillis.h>

float phase = 0.0;
float twopi = 3.14159 * 2;
elapsedMicros usec = 0;

void setup() {
  Serial.begin(115200);
  analogWriteResolution(10); // ATSAMD21 has 10 bit DAC
}

void loop() {
  // put your main code here, to run repeatedly:
  //analogWrite(A0, 126);

  //float val = sin(phase) * 2000.0 + 2050.0;
  float val = sin(phase) * 500 + 500;
  analogWrite(A0, (int)val);
  Serial.print(millis());
  Serial.print(": ");
  Serial.println((int)val);
  phase = phase + 0.02;
  if (phase >= twopi) phase = 0;
  while (usec < 1000) ; // wait
  usec = usec - 1000;
}

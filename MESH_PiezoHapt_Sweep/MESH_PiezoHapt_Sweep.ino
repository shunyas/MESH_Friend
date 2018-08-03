/*
Modify Yurikleb_DRV2667.c so that it doesn't wait for Serial

void Yurikleb_DRV2667::i2c_Scan(){
  // Leonardo: wait for serial port to connect
  // while (!Serial)
  // {
  // }

  Serial.println ();
  Serial.println ("I2C scanner. Scanning ...");
  byte count = 0;

  Wire.begin();
  for (byte i = 8; i < 120; i++)
  {
    Wire.beginTransmission (i);
    if (Wire.endTransmission () == 0)
    {
      Serial.print ("Found address: ");
      Serial.print (i, DEC);
      Serial.print (" (0x");
      Serial.print (i, HEX);
      Serial.println (")");
      count++;
      delay (1);
    }
  }
  Serial.println ("Done.");
  Serial.print ("Found ");
  Serial.print (count, DEC);
  Serial.println (" device(s).");
  Serial.println ("***********");
  Serial.println (" ");

}


And comment out Serial.begin
We will run Serial.begin in setup

  void Yurikleb_DRV2667::begin() {
    Wire.begin();
    //Serial.begin (9600);
    i2c_Scan(); // Scan for i2c devices to make sure we have the DRV2667 attached;
    delay(300);
  }

*/

#include "Yurikleb_DRV2667.h"

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

Yurikleb_DRV2667 drv;

//****** SAMPLE WAVE FORMS *******

// WaveForm Array: [Amplitude, Freq, Cycles, Envelope]
// Amplitude    --  min:0=50v max: 255=100v
// Frequerncy   --  0-255 or 0x00-0xFF
// Duration     --  Cycles 0-255
// Envelope     --  (Ramp up + down)
// Max 60 waves per array !!

byte WaveForm_1[4][4] = {
                      {255, 0x15, 50, 0x09},
                      {255, 0x17, 50, 0x09},
                      {255, 0x19, 50, 0x09},
                      {255, 0x1B, 50, 0x09}
                      };

byte WaveForm_2[6][4] = {
                      {255, 0x15, 50, 0x09},
                      {255, 0x17, 50, 0x09},
                      {255, 0x19, 50, 0x09},
                      {255, 0x1B, 50, 0x09},
                      {255, 0x1D, 50, 0x09},
                      {255, 0x1F, 50, 0x09}
                      };

byte WaveForm_3[2][4] = {
                      {255, 0x15, 50, 0x09},
                      {255, 0x17, 50, 0x09}
                      };

byte WaveForm_4[1][4] = {
                      {8, 0x15, 50, 0x09}
                      };

byte WaveForm[1][4] = {
                      {8, 0x15, 50, 0x09}
                      };

//****** END OF WAVEFORMS ******

void setup() {
  Serial.begin(115200);
  analogWriteResolution(10); // ATSAMD21 has 10 bit DAC
  for (int i=0; i<3; i++){
    int pin = dis[i];
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
  }

  pinMode(do1, INPUT);
  pinMode(do2, INPUT);
  pinMode(do3, INPUT);
  pinMode(pwmo, INPUT);

  drv.begin();
  //drv.setToAnalogInput();  //Swithch To Analog
}

void loop() {
  // test_analog_in();
  // test_din();
  // test_dout();
  // test_pwmo();

  sweep_hapt();
  delay(3000); //Wait for the wave to play;

}

void sweep_hapt(void){
  // WaveForm Array: [Amplitude, Freq, Cycles, Envelope]
  // Amplitude    --  min:0=50v max: 255=100v
  // Frequerncy   --  0-255 or 0x00-0xFF
  // Duration     --  Cycles 0-255
  // Envelope     --  (Ramp up + down)
  byte amps[] = {255, 128, 64, 32, 16, 8, 4, 2};
  byte freqs[] = {255, 128, 64, 32, 16, 8, 4, 2};
  for (int i=0; i < sizeof(freqs); i++){
    for (int j=0; j < sizeof(amps); j++){
      Serial.print("freq: ");
      Serial.print(freqs[i]);
      Serial.print(", ");
      Serial.print("amp: ");
      Serial.println(amps[j]);
      WaveForm[0][0] = amps[j];
      WaveForm[0][1] = freqs[i];
      drv.playWave(WaveForm, sizeof(WaveForm)); //Play one the Waveforms defined above;
      delay(1000);
    }
  }

}

void test_hapt(void){
  int val = digitalRead(do1);
  // Serial.println(val);
  if (val != val1) {
    Serial.println(val);
    if (val == HIGH){
      drv.playWave(WaveForm_4, sizeof(WaveForm_4)); //Play one the Waveforms defined above;
      delay(3000); //Wait for the wave to play;
    }
    val1 = val;
  }
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

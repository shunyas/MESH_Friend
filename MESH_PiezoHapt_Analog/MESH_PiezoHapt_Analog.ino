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
  drv.setToAnalogInput();  //Swithch To Analog

  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
}

void loop() {
}

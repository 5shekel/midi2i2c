///////////////////////////////
// using the audio module
// http://www.pjrc.com/store/teensy3_audio.html
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

AudioPlaySdWav           playSdWav1;
AudioOutputI2S           i2s1;
AudioConnection          patchCord1(playSdWav1, 0, i2s1, 0);
AudioConnection          patchCord2(playSdWav1, 1, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;
///////////////////////////////

///////////////////////////////
/*
#include <AltSoftSerial.h>
AltSoftSerial vixSerial;
*/
///////////////////////////////


///////////////////////////////
#include <Adafruit_NeoPixel.h>
#define ledPIN 17
Adafruit_NeoPixel strip = Adafruit_NeoPixel(32, ledPIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

///////////////////////////

#define MAX_CHANNELS 14
int ch;
int state;
int firstcall;
long prevMillis, delta;
int chVal[MAX_CHANNELS] = {0};
int prevChVal[MAX_CHANNELS] = {0};

enum states
{
  IDLE,
  DELIM,
  READ,
  PROCESS,
  STORE
};
//////////////////////////

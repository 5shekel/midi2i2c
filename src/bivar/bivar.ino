//neopixel lib starndtest + audio board tester for teensy
//uncomment all for audio

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

////////////////////////////
#include <Adafruit_NeoPixel.h>
#define PIN 2
Adafruit_NeoPixel strip = Adafruit_NeoPixel(32, PIN, NEO_GRB + NEO_KHZ800);
////////////////////////////

/////////////////////////////
#define MAX_CHANNELS 15
#include <PololuMaestro.h>
#include <SoftwareSerial.h>
SoftwareSerial srvSerial(3, 4); // RX, TX
MiniMaestro maestro(srvSerial);

#include <PrintEx.h>
using namespace ios;
///////////////////////////

///////////////////////////
int ch;
int state;
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

void setup() {
  Serial.begin(57600);
  srvSerial.begin(57600);

  //AUDIO
  AudioMemory(8);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  SPI.setMOSI(7);
  SPI.setSCK(14);
  if (!(SD.begin(10))) {
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
  delay(1000);

  //LEDS
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  //vixen
  state = IDLE;
  ch = 0;
}

void loop() {

  if (Serial.available())
  {
    switch (state)
    {
      case IDLE:
        ch = 0;
        if (Serial.read() == '+')
        {
          state = DELIM;
        }
        else
        {
          state = IDLE;
        }
        break;

      case DELIM:
        ch = 0;
        if (Serial.read() == '>')
        {
          state = READ;
        }
        else
        {
          state = IDLE;
        }
        break;

      case READ:
        chVal[ch++] = Serial.read();
        if (ch >= MAX_CHANNELS)
        {
          ch = 0;
          state = PROCESS;
        }
        break;

      case PROCESS:
        state = IDLE;

        if (compareArrays(prevChVal, chVal, sizeof(prevChVal))) {
          srvSerial << "same" << endl;
          break; //only continue if the array is diffrent;

        }
        memcpy(prevChVal, chVal, sizeof(prevChVal));

        for (int iii = 0; iii < MAX_CHANNELS; iii++)
          srvSerial << iii << ":" << chVal[iii] << " | "; //print debug out

        srvSerial << endl;
        break;

      case STORE:

        break;
    }
  }
  
  /*
    if (playSdWav1.isPlaying() == false) {
    playSdWav1.play("sosimple.wav");
    delay(10); // wait for library to parse WAV info
    }

    rainbow(20);
  
  maestro.setTargetMiniSSC(servo[1], 255);
  */
}



int compareArrays(int a[], int b[], int n) {
  for (int ii = 0; ii < n; ii++) {
    if (a[ii] != b[ii]) return 0;
    srvSerial << "not smae" << endl;
  }
  return 1;
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

uint32_t Wheel(byte WheelPos) {
  // Input a value 0 to 255 to get a color value.
  // The colours are a transition r - g - b - back to r.
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

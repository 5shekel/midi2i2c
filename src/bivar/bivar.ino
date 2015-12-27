//neopixel lib starndtest + audio board tester for teensy
//uncomment all for audio

/*
  serial for vixen,- pins 3 4 (TX,RX)
  serial1 for pololu servo controller- pins 0 1 (RX1,TX1)
  50 WS2812B LEDs - pin 20
  teensy audio adaptor - pins 6,9,11,13,15,18,19,22,23 (18,19 i2c)
  sd card via [audio adaptor]- pins 7, 10 12, 14 (SPI)
*/

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

#include <Adafruit_NeoPixel.h>
#define PIN 20
Adafruit_NeoPixel strip = Adafruit_NeoPixel(32, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

///////////////////////////

#define MAX_CHANNELS 15
int ch;
int state;
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

void setup() {
  Serial.begin(57600);
  Serial1.begin(57600);
  long unsigned debug_start = millis ();
  while (!Serial && ((millis () - debug_start) <= 7000)) ;
  Serial1.println("working");

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

  //LED
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  //vixen
  state = IDLE;
  ch = 0;
}

void loop() {

  //AUDIO
  if (playSdWav1.isPlaying() == false) {
    playSdWav1.play("sosimple.wav");
    delay(10); // wait for library to parse WAV info
  }
  //LED
  rainbow(20);

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

        if (array_cmp(prevChVal, chVal, MAX_CHANNELS, MAX_CHANNELS)) {
          //Serial1.println("same");
          for ( int i = 0 ; i < MAX_CHANNELS ; ++i )
            prevChVal[ i ] = chVal[ i ];
          break; //only continue if the array is diffrent;
        }

        //copy array
        for ( int i = 0 ;  i < MAX_CHANNELS ; ++i )
          prevChVal[ i ] = chVal[ i ];

        delta = millis() - prevMillis;
        Serial1.print(delta);
        Serial1.print("=");

        for (int iii = 0; iii < MAX_CHANNELS; iii++) {
          Serial1.print(iii);
          Serial1.print(":");
          Serial1.print(chVal[iii]);
          Serial1.print(" | ");
          //Serial1 << iii << ":" << chVal[iii] << " | "; //print debug out
        }
        Serial1.println();
        break;

      case STORE:

        break;
    }
  }
}
boolean array_cmp(int *a, int *b, int len_a, int len_b) {
  int n;
  // if their lengths are different, return false
  if (len_a != len_b) return false;

  // test each element to be the same. if not, return false
  for (n = 0; n < len_a; n++) if (a[n] != b[n]) {
      return false;
    }

  //ok, if we have not returned yet, they are equal :)
  return true;
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

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
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


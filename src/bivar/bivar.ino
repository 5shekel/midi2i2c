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

///////////////////////////

#define MAX_CHANNELS 15
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

        if (array_cmp(prevChVal, chVal, 5, 5)) {
          //Serial1.println("same");
          break; //only continue if the array is diffrent;
        }

        memcpy(prevChVal, chVal, sizeof(prevChVal));

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

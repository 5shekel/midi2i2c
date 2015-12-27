//neopixel lib starndtest + audio board tester for teensy
//uncomment all for audio

/*
  serial for vixen,- pins 3 4 (TX,RX)
  serial1 for pololu servo controller- pins 0 1 (RX1,TX1)
  50 WS2812B LEDs - pin 20
  teensy audio adaptor - pins 6,9,11,13,15,18,19,22,23 (18,19 i2c)
  sd card via [audio adaptor]- pins 7, 10 12, 14 (SPI)
*/

#include "header.h"

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

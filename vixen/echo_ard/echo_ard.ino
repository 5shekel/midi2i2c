//vixen control of servos
//pwm led code via http://freaklabs.org/index.php/blog/chibi/wireless-lighting-control-using-arduino-and-vixen.html


#define MAX_CHANNELS 15

#include <PrintEx.h>
using namespace ios;

#include <SoftwareSerial.h>
SoftwareSerial srvSerial(7, 8); // RX, TX

#include <SPI.h>
#include <SD.h>
const uint8_t chipSelect = 4;

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

void setup()
{
  state = IDLE;
  ch = 0;

  Serial.begin(57600);
  srvSerial.begin(57600);

  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    return;
  }
  Serial.println("card initialized.");

}

void loop()
{
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
}

int compareArrays(int a[], int b[], int n) {
  for (int ii = 0; ii < n; ii++) {
    if (a[ii] != b[ii]) return 0;
    srvSerial << "not smae"<<endl;
  }
  return 1;
}

//vixen control of servos
//pwm led code via http://freaklabs.org/index.php/blog/chibi/wireless-lighting-control-using-arduino-and-vixen.html

#include <SoftwareSerial.h>

#define MAX_CHANNELS 3

SoftwareSerial srvSerial(9, 8); // RX, TX

int ch;
int state;
int chVal[MAX_CHANNELS] = {0};
int prevChVal[MAX_CHANNELS] = {0};

enum states
{
  IDLE,
  DELIM,
  READ,
  DISP
};

void setup()
{
  state = IDLE;
  ch = 0;

  Serial.begin(57600);
  srvSerial.begin(57600);

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
          state = DISP;
        }
        break;

      case DISP:
        state = IDLE;
        if (compareArrays(prevChVal, chVal, MAX_CHANNELS))
          break; //only continue if the array is diffrent;
        for (int iii = 0; iii < MAX_CHANNELS; iii++) {
          srvSerial.print(chVal[iii]);
          srvSerial.print(" ");
          prevChVal[iii] = chVal[iii] ;
        }
        srvSerial.println();
        break;
    }
  }
}

int compareArrays(int a[], int b[], int n) {
  for (int ii = 0; ii < n; ii++) {
    if (a[ii] != b[ii]) return 0;
  }
  return 1;
}

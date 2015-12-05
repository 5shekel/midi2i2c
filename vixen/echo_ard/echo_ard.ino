//vixen control of servos
//pwm led code via http://freaklabs.org/index.php/blog/chibi/wireless-lighting-control-using-arduino-and-vixen.html

#include <SoftwareSerial.h>

#define MAX_CHANNELS 3

SoftwareSerial srvSerial(9, 8); // RX, TX

int ch;
int state;
int chVal[MAX_CHANNELS + 2] = {0};

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
    /*
      chVal[ch++] = Serial.read();

      if (ch >= MAX_CHANNELS + 2) {
      for (ch = 2; ch < MAX_CHANNELS + 2; ch++) {
        srvSerial.print(chVal[ch]);
        srvSerial.print(" ");
      }
      srvSerial.println();
      ch = 0 ;
      }
    */

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
        for (int iii = 0; iii < MAX_CHANNELS; iii++) {
          srvSerial.print(chVal[iii]);
          srvSerial.print(" ");
        }
        srvSerial.println();

        break;
    }

  }
}

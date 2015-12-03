# midi2i2c
a micro light movement and sound show.

spec:
12 x 360 servos controlled by i2c
12 x RGB addressable led controlled by i2c
1   x mp3 module controlled by UART

code:
* assign each element a MIDI address based on its i2c address
* 3rd party should be able to control all elemnts from a MIDI sequence program (like ableton live). 
* bi directional rotation for the motors, color change for the LEDs and play/stop for the music.

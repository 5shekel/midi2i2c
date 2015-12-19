a Light,Movement and Sound show(control).

unlike its name we dont use midi ...   lots of switching hardware, fails and smoke got me to current setup so remmber kids, name your project by spirit not by its limbs.  

just serial from vixen showcontrol and out to the servo controler(serial), leds(i2c) and music(i2c,SPI)

spec:
12 x 360 servos controlled by serial
70 x RGB addressable led controlled by i2c  
1   x audio module controlled by i2c  
 
code: 
* assign each element an address
* 3rd party should be able to control all elemnts from a sequence program (like ableton live, or [vixen](http://freaklabs.org/index.php/blog/chibi/wireless-lighting-control-using-arduino-and-vixen.html)). 
* bi directional rotation for the motors, color change for the LEDs and play/stop for the music.

serial for [vixen](http://www.vixenlights.com/),- pins 0 1  
serial1 for pololu [servo controller](https://www.pololu.com/docs/0J40)- pins 3 4  
50 [WS2812B](http://i.imgur.com/nXHc4Y4.png) LEDs - pin 20  
teensy [audio adaptor](https://www.pjrc.com/store/teensy3_audio.html) - pins 6,9,11,13,15,18,19,22,23 (18,19 i2c)  
sd card via [audio adaptor]- pins 7, 10 12, 14 (SPI)

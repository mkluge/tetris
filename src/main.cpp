#include <Adafruit_NeoPixel.h>



#define PIN D2
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 5

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  pixels.begin();
  randomSeed(0);
}

void loop()
{
  static int light=255;
  static int pixel = (int)random() % NUMPIXELS;
  int delayval = 5; // delay for half a second
  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.

  // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
  pixels.setPixelColor(pixel, pixels.Color(0, 0, light));
  pixels.show();
  if (light == 0) {
    delay(200);
    light = 255;
    int oldpixel = pixel;
    while (pixel == oldpixel) {
      pixel = (int)random() % NUMPIXELS;
    }
  } else
  {
    light--;
    delay(delayval);
  }
    //    pixels.setPixelColor(i, pixels.Color(255, 0, 0));
    //    pixels.show();
    //    delay(delayval);
    //    pixels.setPixelColor(i, pixels.Color(0, 0, 255));
    //    pixels.show();
    //    delay(delayval);
    // }
}
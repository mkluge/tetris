#include <AdaNeoDisplay.h>
#include <TestAnimation.h>

#define PIN D2
#define PIXELS_X 8
#define PIXELS_Y 12

AdaNeoDisplay display = AdaNeoDisplay( PIXELS_X, PIXELS_Y, PIN);
TestAnimation animation = TestAnimation( display, 0, 0);

void setup()
{
}

void loop()
{

  int delayval = 5;

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
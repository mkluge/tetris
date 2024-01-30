#include <AdaNeoDisplay.h>
#include <TestAnimation.h>

#define PIN D2
#define PIXELS_X 8
#define PIXELS_Y 12

AdaNeoDisplay display = AdaNeoDisplay( PIXELS_X, PIXELS_Y, PIN);
TestAnimation animation = TestAnimation( display, 5, 5);

void setup()
{
}

void loop()
{
  animation.paint();
  display.setPixel(0,0,{255,255,255});
}
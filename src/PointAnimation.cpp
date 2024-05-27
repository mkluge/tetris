#include <PointAnimation.h>
#include <stdlib.h>
#include <Arduino.h>

PointAnimation::PointAnimation(LEDDisplay &display, int base_x, int base_y) : Animation(display)
{
    internal_pixels.push_back({base_x, base_y, {0, 255, 255}});
    brightness = 255;
}

PointAnimation::~PointAnimation()
{
}

void PointAnimation::step()
{
}

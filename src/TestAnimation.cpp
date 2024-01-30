#include "TestAnimation.h"

TestAnimation::TestAnimation(LEDDisplay &display, int base_x, int base_y) : Animation(display)
{
    pixels.push_back({base_x, base_y, {255, 0, 255}});
    pixels.push_back({base_x-1, base_y, {255, 255, 0}});
    pixels.push_back({base_x+2, base_y, {255, 255, 0}});
    pixels.push_back({base_x, base_y-1, {0, 255, 255}});
    pixels.push_back({base_x, base_y+2, {0, 255, 255}});
}

TestAnimation::~TestAnimation()
{
}

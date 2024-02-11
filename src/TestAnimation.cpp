#include "TestAnimation.h"
#include <stdlib.h>
#include <Arduino.h>

TestAnimation::TestAnimation(LEDDisplay &display, int base_x, int base_y) : Animation(display)
{
    pixels.push_back({base_x, base_y, {255, 0, 255}});
    pixels.push_back({base_x-1, base_y, {255, 255, 0}});
    pixels.push_back({base_x+1, base_y, {255, 255, 0}});
    pixels.push_back({base_x, base_y-1, {0, 255, 255}});
    pixels.push_back({base_x, base_y+1, {0, 255, 255}});
    for( auto p : pixels) 
    {
        brightness.push_back(255);
    }
}

TestAnimation::~TestAnimation()
{
}

void TestAnimation::step()
{
    std::list<int> new_brightness;
    for (auto &p : pixels)
    {
        auto b = brightness.front();
        brightness.pop_front();
        int flicker = random() % (FLICKER_RANGE*2);
        flicker -= FLICKER_RANGE;
        b += flicker;
        b = max(MIN_BRIGHTNESS, min(MAX_BRIGHTNESS, b));
        new_brightness.push_back(b);
        p.color = {(uint8_t)b, (uint8_t) b, (uint8_t) b};
    }
    brightness = new_brightness;
}

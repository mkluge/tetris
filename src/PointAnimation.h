#ifndef POINTANIMATION_H
#define POINTANIMATION_H

#include <Animation.h>
#include <list>

#pragma once

const int MIN_BRIGHTNESS = 10; // Minimum brightness level
const int MAX_BRIGHTNESS = 255; // Maximum brightness level
const int FLICKER_RANGE = 90;   // Range of flicker variation
const int MIN_FLICKER = 30;
class PointAnimation : public Animation
{
public:
    PointAnimation(LEDDisplay &display, int base_x, int base_y);
    ~PointAnimation();
    void step();
private:
    int brightness;
};

#endif
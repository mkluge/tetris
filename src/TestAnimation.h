#ifndef TESTANIMATION_H
#define TESTANIMATION_H

#include <Animation.h>

#pragma once

class TestAnimation : public Animation
{
public:
    TestAnimation(LEDDisplay &display, int base_x, int base_y);
    ~TestAnimation();
};

#endif
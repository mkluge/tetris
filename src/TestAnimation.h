#ifndef TESTANIMATION_H
#define TESTANIMATION_H

#include <Animation.h>

#pragma once

class TestAnimation : Animation
{
public:
    TestAnimation(LEDDisplay &display, int base_x, int base_y);
    ~TestAnimation();
    

private:
};

#endif
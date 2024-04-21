#ifndef BASIC_H
#define BASIC_H

#include <tuple>

#pragma once

struct Rect {
    int start_x;
    int start_y;
    int end_x;
    int end_y;
};

struct RGB {
    unsigned short red;
    unsigned short green;
    unsigned short blue;    
};

struct Pixel {
    int x;
    int y;
    RGB color;
};
bool operator==(const Pixel &lhs, const Pixel &rhs);

struct Keys
{
    bool left_push;
    bool right_push;
    bool left_joy;
    bool up_joy;
    bool right_joy;
    bool down_joy;
};

RGB ZeroOneToRainbow(double f);

#endif
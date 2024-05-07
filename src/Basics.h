#ifndef BASIC_H
#define BASIC_H

#include <tuple>

#pragma once

// input pins
#define DISPLAY_PIN 32
#define D_JOYSTICK_PIN 3
#define R_JOYSTICK_PIN 21
#define U_JOYSTICK_PIN 19
#define L_JOYSTICK_PIN 18
#define R_PUSH_PIN 22
#define L_PUSH_PIN 23
#define BUTTON_LEFT_LED 1
#define BUTTON_RIGHT_LED 17
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

RGB ZeroOneToRainbow(double f);

#endif
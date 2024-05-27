#ifndef BASIC_H
#define BASIC_H

#include <tuple>
#include <list>

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

// some colors
#define TYELLOW {0xFF, 0xFF, 0x00}
#define TGREEN {0x00, 0xFF, 0x00}
#define TRED {0xFF, 0x00, 0x00}
#define TBLUE {0x00, 0x00, 0xFF}
#define TORANGE {0xFF, 0xA5, 0X00}
#define TPINK {0xFF, 0x69, 0xB4}
#define TBROWN {0x8B, 0x45, 0x13}
struct Pixel {
    int x;
    int y;
    RGB color;
};
typedef std::list<Pixel> PixelList;
bool operator==(const Pixel &lhs, const Pixel &rhs);
typedef void (*animation)();

RGB ZeroOneToRainbow(double f);

#endif
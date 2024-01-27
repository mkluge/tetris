#ifndef BASICS_H
#define BASICS_H

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

#endif
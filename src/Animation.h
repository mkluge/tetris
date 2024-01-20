#ifndef ANIMATION_H
#define ANIMATION_H

#include <AnimatedDisplay.h>

#pragma once

struct Rect{
    int start_x;
    int start_y;
    int end_x;
    int end_y;
};

struct Animation
{
    virtual void start(int base_x, int base_y) = 0;
    virtual void animate() = 0;
    virtual void rotate(int direction, bool keepInside=false) = 0;
    virtual void translate(int x, int y, bool keepInside=false) = 0; 
    virtual void setColor( int red, int green, int blue) = 0;
    virtual Rect boundingBox() = 0 ;
    // for tetris
    virtual void removeLine() = 0;
};

#endif
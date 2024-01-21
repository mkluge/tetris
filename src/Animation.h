#ifndef ANIMATION_H
#define ANIMATION_H

#include <Basics.h>
#include <LEDDisplay.h>

#pragma once

// rotation matrix in 2D is
// R = ( cos(a) -sin(a))
//     ( sin(a)  cos(a))
// so 90 degrees is
// R = ( 0 -1 ) -> -y+x
//     ( 1  0 )
// -90 dregees is:
// R = ( 0  1 ) -> y-x
//     ( -1 0 )

class Animation
{
public:
    Animation(LEDDisplay &display, int base_x, int base_y);
    virtual void animate() = 0;
    virtual void rotate(int direction, bool keepInside = false) = 0;
    virtual void translat(int x, int y, bool keepInside = false) = 0;
    virtual void setColor(int red, int green, int blue) = 0;
    virtual Rect boundingBox() = 0;
    // for tetris
    virtual void removeLine() = 0;

private:
    int base_x;
    int base_y;
    LEDDisplay &display;
};

#endif
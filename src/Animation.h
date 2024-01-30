#ifndef ANIMATION_H
#define ANIMATION_H

#include <Basics.h>
#include <LEDDisplay.h>
#include <list>

#pragma once

class Animation
{
public:
    enum Rotation
    {
        LEFT,
        RIGHT,
    };

    Animation(LEDDisplay &display);
    virtual void rotate(Rotation direction, bool keepInside = false);
    virtual void translate(int x, int y, bool keepInside = false);
    virtual void setColor(unsigned short red, unsigned short green, unsigned short blue);
    virtual void moveInside();
    virtual Rect boundingBox();
    virtual void paint();
    // for Tetris
    virtual void removeLine(int line);

protected:
    LEDDisplay &display;
    std::list<Pixel> pixels;
};

#endif
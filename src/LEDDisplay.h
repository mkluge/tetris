#ifndef LEDDISPLAY_H
#define LEDDISPLAY_H

#include <Basics.h>

#pragma once

class LEDDisplay
{
public:
    virtual void setPixel( const int x, const int y, RGB value) = 0;
    virtual void show() = 0;
    virtual void clear() = 0;
    virtual const int width() const = 0;
    virtual const int height() const = 0;
};

#endif
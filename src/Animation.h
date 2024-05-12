#ifndef ANIMATION_H
#define ANIMATION_H

#include <Basics.h>
#include <LEDDisplay.h>
#include <list>

#pragma once
class Animation
{
public:
    typedef std::list<Pixel> PixelList;
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
    virtual const Rect boundingBox() const;
    virtual void paint() const;
    virtual void unpaint() const;
    virtual void addPixels(const PixelList &toadd);
    virtual void removePixels(const PixelList &toremove);
    virtual const PixelList &getPixels();
    // for Tetris
    void removeLine(int line);

protected:
    LEDDisplay &display;
    PixelList pixels;
};

#endif
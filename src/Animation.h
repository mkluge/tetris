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
    virtual bool canTranslate(int x, int y, Animation *mayCrashInto);
    virtual bool canRotate(Animation *mayCrashInto);
    virtual void useCachePixels();
    virtual void setColor(unsigned short red, unsigned short green, unsigned short blue);
    virtual void moveInside(Animation *mayCrashInto);
    virtual const Rect boundingBox() const;
    virtual void paint() const;
    virtual const bool pixelOutsideScreen( int x, int y) const;
    virtual const bool pixelInside( int x, int y, Animation *mayCrashInto) const;
    virtual void unpaint() const;
    virtual void addPixels(const PixelList &toadd);
    virtual void removePixels(const PixelList &toremove);
    virtual const PixelList getPixels() const;
    virtual void clearPixels();

protected:
    LEDDisplay &display;
    PixelList internal_pixels;
    PixelList cache_pixels;
    int offset_x;
    int offset_y;
    int display_width;
    int display_height;
};

#endif
#ifndef ADANEODISPLAY_H
#define ADANEODISPLAY_H

#include <LEDDisplay.h>
#include <Adafruit_NeoPixel.h>

#pragma once

class AdaNeoDisplay : LEDDisplay
{
public:
    AdaNeoDisplay( const int size_x, const int size_y, const int pin);
    ~AdaNeoDisplay();
    void setPixel(const int x, const int y, const RGB value);
    void show();
    void clear();
    const int width() const;
    const int height() const;

private:
    int size_x;
    int size_y;
    int numpixels;
    int pin;
    Adafruit_NeoPixel pixels;
};

#endif
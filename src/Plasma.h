#ifndef PLASMA_H
#define PLASMA_H

#include <Arduino.h>
#include <LEDDisplay.h>
#include <array>
#include <map>
#include <FastTrig.h>
#include <math.h>
#pragma once

template <int size_x, int size_y>
class Plasma
{
public:
    Plasma(LEDDisplay &display) : display(display)
    {
    }

    ~Plasma()
    {
    }

    void init()
    {
        plasma_time = 1;
        speed = 10;
    };

    // Function to calculate plasma color for a given pixel
    RGB calculatePlasmaColor(int x, int y, int time, double direction)
    {
        unsigned short r, g, b;
        double t = time * 0.05;             // Time parameter for animation
        double dx = speed * cos(direction); // Calculate horizontal movement
        double dy = speed * sin(direction); // Calculate vertical movement
        double v = sin((x + dx) * 0.3 + (y + dy) * 0.2 + t) +
                   sin((x + dx) * 0.4 + (y + dy) * 0.3 + t) +
                   sin((x + dx + y + dy) * 0.5 + t) +
                   sin(sqrt(double((x + dx) * (x + dx) + (y + dy) * (y + dy))) * 0.7 + t);
        r = int(128 + 127 * sin(PI * v));
        g = int(128 + 127 * sin(PI * (v + 2.094)));
        b = int(128 + 127 * sin(PI * (v + 4.188)));
        return {r,g,b};
    }

    // Function to calculate plasma color for a given pixel
    RGB calculatePlasmaColor2(int x, int y, int time)
    {
        unsigned short r, g, b;
        // scale x and y so that they move between 0 and 180
        double v = isin256(x*x) + isin256(y*y) + isin(x+y) + (time%360);
        r = int(128 + 127 * isin(v));
        g = int(128 + 127 * isin(v + 120));
        b = int(128 + 127 * isin(v + 240));
        return {r, g, b};
    }

    void paint()
    {
        for (auto x = 0; x < size_x; x++)
        {
            for (auto y = 0; y < size_y; y++)
            {
                display.setPixel(x, y, calculatePlasmaColor2(x, y, plasma_time));
            }
        }
        display.show();
        plasma_time += speed;
    }

private:
    LEDDisplay &display;
    double plasma_time, speed;
};

#endif
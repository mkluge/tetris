#ifndef PLASMA_H
#define PLASMA_H

#include <Arduino.h>
#include <LEDDisplay.h>
#include <array>
#include <map>
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
        speed = 1;
    };

    // Function to calculate plasma color for a given pixel
    RGB calculatePlasmaColor(int x, int y, int time)
    {
        unsigned short r, g, b;
        double t = time * 0.05; // Time parameter for animation
        double v = sin(x * 0.3 + t) + sin(y * 0.4 + t) + sin((x + y) * 0.5 + t) +
                   sin(sqrt(double(x * x + y * y)) * 0.7 + t);
                   sin(sqrt(double(x * x + y * y)) * 0.7 + t);
        r = (unsigned int)(128 + 127 * sin(PI * (v)));
        g = (unsigned int)(128 + 127 * sin(PI * (v + 2.094)));
        b = (unsigned int)(128 + 127 * sin(PI * (v + 4.188)));
        return {r,g,b};
    }
    void paint()
    {
        for (auto x = 0; x < size_x; x++)
        {
            for (auto y = 0; y < size_y; y++)
            {
                display.setPixel(x, y, calculatePlasmaColor(x, y, plasma_time));
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
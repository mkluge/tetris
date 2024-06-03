#ifndef NEO_H
#define NEO_H

#include <Arduino.h>
#include <LEDDisplay.h>
#include <array>
#include <map>
#include <FastTrig.h>
#include <math.h>
#pragma once

template <int size_x, int size_y>
class Neo
{
public:
    Neo(LEDDisplay &display) : display(display)
    {
    }

    ~Neo()
    {
    }

    void init()
    {
        neo_time = 1;
        speed = 10;
    };

    // Function to calculate plasma color for a given pixel
    RGB calculateNeoColor(int x, int y, int time, double direction)
    {
        unsigned short r, g, b;
        double t = time * 0.05;             // Time parameter for animation
        //double dx = speed * cos(direction); // Calculate horizontal movement
        double dy = speed * sin(direction); // Calculate vertical movement
        double v = sin((y + dy) * 0.2 + t) +
                   sin((y + dy) * 0.3 + t) +
                   sin((y + dy) * 0.5 + t) +
                   sin(sqrt(double((y + dy) * (y + dy))) * 0.7 + t);
        r = int(128 + 127 * sin(PI * v));
        g = int(128 + 127 * sin(PI * (v + 2.094)));
        b = int(128 + 127 * sin(PI * (v + 4.188)));
        return {r,g,b};
    }

    // Function to calculate plasma color for a given pixel
    RGB calculateNeoColor2(int x, int y, int time)
    {
        unsigned short r, g, b;
        // scale x and y so that they move between 0 and 180
        double v = isin256(y*y) + isin(y) + (time%360);
        r = int(0);
        g = int(128 + 127 * isin(v + 120));
        b = int(0);
        return {r, g, b};
    }

    void paint()
    {
        for (auto i = 0; i < size_y/2; i++)
        {
            int x = int(rand() % size_x);
            auto xc = (rand() % 255) - 255;
            for (auto y = 0; y < size_y/i; y++)
            {
                display.setPixel(x, y, calculateNeoColor2(xc, y, neo_time));
            }
        }
        display.show();
        neo_time += speed;
    }

private:
    LEDDisplay &display;
    double neo_time, speed;
};

#endif

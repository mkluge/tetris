#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <Arduino.h>
#include <LEDDisplay.h>
#include <array>
#include <map>
#include <math.h>

#pragma once

template <int size_x, int size_y>
class Mandelbrot
{
public:
    Mandelbrot(LEDDisplay &display) : display(display)
    {
    }

    ~Mandelbrot()
    {
    }

    void init()
    {
        t1 = millis();
        t2 = millis();

        cX = cR + t1;
        cY = cI + (t1 / 2.5);
    };

    RGB render2D(int x, int y)
    {
        return ZeroOneToRainbow( (double)(x+(y*(size_x))) / (double)((size_x) * (size_y)));
    }

    RGB ZeroOneToRainbow(double f)
    {
        double a = (1 - f) / (double)0.2;
        double X = floor(a);
        unsigned short Y = (255.0*a) -(255.0*X);
        unsigned short RY = 255-Y;
        switch ((int)X)
        {
        case 0:
            return {255,Y,0};
        case 1:
            return {RY,255,0};
        case 2:
            return {0,255,Y};
        case 3:
            return {0,RY,255};
        case 4:
            return {Y,0,255};
        case 5:
        default:
            return {255,0,255};
        }
    }

    void paint()
    {
        for (auto x = 0; x < size_x; x++)
        {
            for (auto y = 0; y < size_y; y++)
            {
                auto color = render2D(x, y);
                display.setPixel(x, y, color);
            }
        }
        display.show();
    }

private:
    LEDDisplay &display;
    int maxIterations = 15;
    double t1, t2, cX, cY;
    double cR = -0.94299;
    double cI = 0.3162;
};

#endif
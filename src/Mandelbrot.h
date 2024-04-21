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
    };

    int mandelbrot(double real, double imag)
    {
        int limit = 100;
        double zReal = real;
        double zImag = imag;

        for (int i = 0; i < limit; ++i)
        {
            double r2 = zReal * zReal;
            double i2 = zImag * zImag;

            if (r2 + i2 > 4.0)
                return i;

            zImag = 2.0 * zReal * zImag + imag;
            zReal = r2 - i2 + real;
        }
        return limit;
    }

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
        double dx = (x_fin - x_start) / (size_x - 1);
        double dy = (y_fin - y_start) / (size_y - 1);

        for (auto x = 0; x < size_x; x++)
        {
            for (auto y = 0; y < size_y; y++)
            {
                double rx = x_start + x * dx; // current real value
                double ry = y_fin - y * dy;   // current imaginary value

                int value = mandelbrot(rx, ry);
                display.setPixel(x,y,ZeroOneToRainbow((double)value/100.0));
                if(value==100)
                {
                    display.setPixel(x,y,{0,0,0});
                }
            }
        }
        display.show();
        x_start /= 1.01;
        y_start /= 1.01;
    }

private:
    LEDDisplay &display;
    double x_start = -2.0;
    double x_fin = 1.0;
    double y_start = -1.0;
    double y_fin = 1.0;
};

#endif
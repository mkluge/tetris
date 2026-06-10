#ifndef PLASMA_H
#define PLASMA_H

#include <Arduino.h>
#include <LEDDisplay.h>
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

    void init()
    {
        plasma_time = 1;
        speed = 4;
    }

    RGB colorWheel(int hue, int brightness)
    {
        hue %= 768;
        if (hue < 0)
        {
            hue += 768;
        }

        int r = 0;
        int g = 0;
        int b = 0;

        if (hue < 256)
        {
            r = 255 - hue;
            g = hue;
        }
        else if (hue < 512)
        {
            hue -= 256;
            g = 255 - hue;
            b = hue;
        }
        else
        {
            hue -= 512;
            b = 255 - hue;
            r = hue;
        }

        r = (r * brightness) / 255;
        g = (g * brightness) / 255;
        b = (b * brightness) / 255;
        return {static_cast<unsigned short>(r),
                static_cast<unsigned short>(g),
                static_cast<unsigned short>(b)};
    }

    RGB calculatePlasmaColor(int x, int y, int time)
    {
        int cx = x * 32 - (size_x * 16);
        int cy = y * 32 - (size_y * 16);

        int main_wave = isin256((x * 42 + y * 22 + time * 9) * 2);
        int cross_wave = isin256((x * -18 + y * 44 + time * 7 + 720) * 2);
        int vertical_swell = isin256(y * 58 - time * 8 + isin(time * 0.5) * 18 + 720);
        int center_ripple = isin256((abs(cx) + abs(cy)) * 9 - time * 10 + 720);

        int wave = (main_wave * 4 + cross_wave * 2 + vertical_swell * 2 + center_ripple) / 9;
        int hue = 300 + (wave / 3) + time * 2 + y * 10;
        int brightness = 120 + ((wave + 256) / 4);

        brightness = constrain(brightness, 65, 245);
        return colorWheel(hue, brightness);
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
    double plasma_time = 1;
    double speed = 4;
};

#endif

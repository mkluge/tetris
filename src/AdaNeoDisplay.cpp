#include "AdaNeoDisplay.h"

AdaNeoDisplay::AdaNeoDisplay(Adafruit_NeoPixel &display, const int size_x, const int size_y) : 
    display(display), size_x(size_x), size_y(size_y)
{
    numpixels = size_x * size_y;
}

AdaNeoDisplay::~AdaNeoDisplay()
{
}

void AdaNeoDisplay::setPixel(int x, int y, RGB value)
{
    // zig zag stuff
    int line_offset;
    // 0,0 is left bottom
    if (x % 2) // odd line bottom->top
    {
        line_offset = (((size_x-1) - x) * size_y) + y;
    }
    else // even line -> top->bottom
    {
        line_offset = (((size_x-1) - x) * size_y) + (size_y-1) - y;
    }
    display.setPixelColor(line_offset, display.Color(value.red, value.green, value.blue));
}

void AdaNeoDisplay::delPixel(const int x, const int y)
{
    setPixel( x, y, {0,0,0});
}

void AdaNeoDisplay::start()
{
    display.begin();
}

void AdaNeoDisplay::show()
{
    display.show();
}

void AdaNeoDisplay::clear()
{
    for (int pixel = 0; pixel < numpixels; pixel++)
    {
        display.setPixelColor(pixel, display.Color(0, 0, 0));
    }
    this->show();
}

const int AdaNeoDisplay::width() const
{
    return size_x;
}

const int AdaNeoDisplay::height() const
{
    return size_y;
}


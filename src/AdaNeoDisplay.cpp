#include "AdaNeoDisplay.h"

AdaNeoDisplay::AdaNeoDisplay(int size_x, int size_y, int pin) : size_x(size_x), size_y(size_y), pin(pin)
{
    numpixels = size_x * size_y;
    pixels = Adafruit_NeoPixel(numpixels, pin, NEO_GRB + NEO_KHZ800);
    pixels.begin();
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
    pixels.setPixelColor(line_offset, pixels.Color(value.red, value.green, value.blue));
}

void AdaNeoDisplay::delPixel(const int x, const int y)
{
    setPixel( x, y, {0,0,0});
}

void AdaNeoDisplay::show()
{
    pixels.show();
}

void AdaNeoDisplay::clear()
{
    for (int pixel = 0; pixel < numpixels; pixel++)
    {
        pixels.setPixelColor(pixel, pixels.Color(0, 0, 0));
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

#include "AdaNeoDisplay.h"


AdaNeoDisplay::~AdaNeoDisplay()
{

}

void AdaNeoDisplay::setPixel(int x, int y, RGB value)
{
    // figure out which pixel in the zig zag stuff
    // TODO: correct implementation after assembling
    int pixel = 0;
    pixels.setPixelColor(pixel, pixels.Color(value.red, value.green, value.blue));
}

void AdaNeoDisplay::show()
{
    pixels.show();    
}

void AdaNeoDisplay::clear()
{
    for( int pixel = 0; pixel<numpixels; pixel++)
    {
        pixels.setPixelColor(pixel, pixels.Color(0, 0, 0));
    }
    this->show();
}

int AdaNeoDisplay::width()
{
    return size_x;
}

int AdaNeoDisplay::height()
{
    return size_y;
}

AdaNeoDisplay::AdaNeoDisplay(int size_x, int size_y, int pin): 
size_x(size_x), size_y(size_y), pin(pin)
{
    numpixels = size_x * size_y;
    pixels = Adafruit_NeoPixel( numpixels, pin, NEO_GRB + NEO_KHZ800);
    pixels.begin();
}

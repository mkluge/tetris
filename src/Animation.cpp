#include "Animation.h"

Animation::Animation(LEDDisplay &display) : display(display)
{
}

void Animation::rotate(Rotation direction, bool keepInside)
{
    // rotation matrix in 2D is
    // R = ( cos(a) -sin(a))
    //     ( sin(a)  cos(a))
    // so 90 degrees is
    // R = ( 0 -1 ) -> x = -y ; y = x
    //     ( 1  0 )
    // -90 dregees is:
    // R = ( 0  1 ) -> x = y ; y = -x
    //     ( -1 0 )
    for (auto &pixel : pixels)
    {
        if (direction == RIGHT)
        {
            auto oldx = pixel.x;
            pixel.x = -pixel.y;
            pixel.y = oldx;
        }
        if (direction == LEFT)
        {
            auto oldx = pixel.x;
            pixel.x = pixel.y;
            pixel.y = -oldx;
        }
    }
    if (keepInside)
    {
        moveInside();
    }
}

void Animation::translate(int move_x, int move_y, bool keepInside)
{
    for (auto &pixel : pixels)
    {
        pixel.x += move_x;
        pixel.y += move_y;
    }
    if (keepInside)
    {
        moveInside();
    }
}

void Animation::setColor(unsigned short red, unsigned short green, unsigned short blue)
{
    for (auto &pixel : pixels)
    {
        pixel.color = {red, green, blue};
    }
}

void Animation::moveInside()
{
    // check all coordinates if they are
    // outside the screen, if -> move everything
    // probably dangerous if animation is wider than
    // the screen

    int move_x = 0;
    int move_y = 0;
    struct Rect bb = boundingBox();
    if (bb.start_x < 0)
    {
        move_x = -bb.start_x;
    }
    else if (bb.end_x >= display.width())
    {
        move_x = -1 - (bb.end_x - display.width());
    }
    if (bb.start_y < 0)
    {
        move_y = -bb.start_y;
    }
    else if (bb.end_y >= display.height())
    {
        move_y = -1 - (bb.end_y - display.height());
    }
    if (move_x != 0 || move_y != 0)
    {
        translate(move_x, move_y, false);
    }
}

const Rect Animation::boundingBox() const
{
    struct Rect retval = {display.width(), display.height(), 0, 0};
    for (auto &pixel : pixels)
    {
        if (pixel.x < retval.start_x)
        {
            retval.start_x = pixel.x;
        }
        if (pixel.y < retval.start_y)
        {
            retval.start_y = pixel.y;
        }
        if (pixel.x > retval.end_x)
        {
            retval.end_x = pixel.x;
        }
        if (pixel.y > retval.end_y)
        {
            retval.end_y = pixel.y;
        }
    }
    return retval;
}

void Animation::paint() const
{
    for (auto &pixel : pixels)
    {
        display.setPixel(pixel.x,
                         pixel.y,
                         pixel.color);
    }
}

void Animation::addPixels(const PixelList &toadd)
{
    for( auto pixel: toadd)
    {
        pixels.push_back(pixel);
    }    
}

void Animation::removePixels(const PixelList &toremove)
{
    for (const auto pixel : toremove)
    {
        pixels.remove(pixel);
    }
}

const Animation::PixelList &Animation::getPixels()
{
    return pixels;
}

/**
 * all lines bigger than the selected one is moved one up
 */
void Animation::removeLine(int line)
{
    std::list<Pixel> new_pixels;
    for (auto &pixel : pixels)
    {
        if (pixel.y < line)
        {
            new_pixels.push_back(pixel);
        }
        // all ==line pixels are removed
        else if (pixel.y > line)
        {
            pixel.y--;
            new_pixels.push_back(pixel);
        }
    }
    pixels = new_pixels;
}

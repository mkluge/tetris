#include "Animation.h"
#include <esp_random.h>

Animation::Animation(LEDDisplay &display) : display(display)
{
    display_width = display.width();
    display_height = display.height();
}

/***
 * calculates if a animation can be rotated clockwise
 * without leaving the screen or crashing into the given
 * animation, leaves calculated new pixels in the internal
 * pixel cache, cache incomplete if function returns false
 */
bool Animation::canRotate(Animation *mayCrashInto)
{
    int direction = RIGHT;
    cache_pixels.clear();
    // rotation matrix in 2D is
    // R = ( cos(a) -sin(a))
    //     ( sin(a)  cos(a))
    // so 90 degrees is
    // R = ( 0 -1 ) -> x = -y ; y = x
    //     ( 1  0 )
    // -90 dregees is:
    // R = ( 0  1 ) -> x = y ; y = -x
    //     ( -1 0 )
    for (auto &pixel : internal_pixels)
    {
        int x, y;
        if (direction == RIGHT)
        {
            x = offset_x - pixel.y;
            y = offset_y + pixel.x;
        }
        if (direction == LEFT)
        {
            x = offset_x + pixel.y;
            y = offset_y - pixel.x;
        }
        if (pixelOutsideScreen(x, y))
        {
            return false;
        }
        if (pixelInside(x, y, mayCrashInto))
        {
            return false;
        }
        cache_pixels.push_back({x, y, pixel.color});
    }
    return true;
}

const bool Animation::pixelOutsideScreen(int x, int y) const
{
    if (x < 0 || x >= display_width)
    {
        return true;
    }
    if (y < 0 || y >= display_height)
    {
        return true;
    }
    return false;
}

const bool Animation::pixelInside(int x, int y, Animation *mayCrashInto) const
{
    auto fpixels = mayCrashInto->getPixels();
    for (auto fpix : fpixels)
    {
        if ((fpix.x == x) && (fpix.y == y))
        {
            return true;
        }
    }
    return false;
}

/**
 * can this Animation move there without going beyond a wall,
 * beyond the ground and without colliding with the floor
 * animation
 */
bool Animation::canTranslate(int x, int y, Animation *mayCrashInto)
{
    for (auto &pixel : internal_pixels)
    {
        // new place for this pixel
        int pixelx = offset_x + pixel.x + x;
        int pixely = offset_y + pixel.y + y;
        // now check conditions
        if (pixelOutsideScreen(x, y))
        {
            return false;
        }
        if (pixelInside( x, y, mayCrashInto))
        {
            return false;
        }
        cache_pixels.push_back({x, y, pixel.color});
    }
    return true;
}

void Animation::setColor(unsigned short red, unsigned short green, unsigned short blue)
{
    for (auto &pixel : internal_pixels)
    {
        pixel.color = {red, green, blue};
    }
}

void Animation::moveInside(Animation *mayCrashInto)
{
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
        if(canTranslate(move_x, move_y, mayCrashInto))
        {
            useCachePixels();
        }
    }
}

void Animation::useCachePixels()
{
    internal_pixels = cache_pixels;
}

const Rect Animation::boundingBox() const
{
    struct Rect retval = {display.width(), display.height(), 0, 0};
    for (auto &pixel : internal_pixels)
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

void Animation::paint(const int color_variation) const
{
    for (auto &pixel : internal_pixels)
    {
        auto color = pixel.color;
        // variate colors up to color_variation for rgb
        if( color_variation!=0 )
        {
            uint32_t rand_num = esp_random();
            // take 10 bits for each pixel
            int red_pixels = (rand_num & 255);
            int green_pixels = (rand_num>>8) & 255;
            int blue_pixels = (rand_num>>16) & 255;
            // so the pixels are always +- original value
            int range = color_variation<<1;
            int red_variation = (red_pixels % range) - color_variation;
            int green_variation = (green_pixels % range) - color_variation;
            int blue_variation = (blue_pixels % range) - color_variation;
            color = {
                (unsigned short)(min3( 0, 255, (int)(color.red + red_variation))),
                (unsigned short)(min3( 0, 255, (int)(color.green + green_variation))),
                (unsigned short)(min3( 0, 255, (int)(color.blue + blue_variation))),
            };
        }
        display.setPixel(pixel.x,
                         pixel.y,
                         color);
    }
}

void Animation::unpaint() const
{
    for (auto &pixel : internal_pixels)
    {
        display.delPixel(pixel.x + offset_x,
                         pixel.y + offset_y);
    }
}

void Animation::addPixels(const PixelList &toadd)
{
    for (auto pixel : toadd)
    {
        internal_pixels.push_back(pixel);
    }
}

void Animation::removePixels(const PixelList &toremove)
{
    for (const auto pixel : toremove)
    {
        internal_pixels.remove(pixel);
    }
}

const PixelList Animation::getPixels() const
{
    PixelList retval;
    for (auto &pixel : internal_pixels)
    {
        retval.push_back({pixel.x + offset_x,
                          pixel.y + offset_y,
                          pixel.color});
    }
    return retval;
}

void Animation::clearPixels()
{
    internal_pixels.clear();
}
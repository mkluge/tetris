#include <TetrisGame.h>
#include <PointAnimation.h>

TetrisGame::TetrisGame(LEDDisplay &display, int width, int height) :
    display(display),
    width(width),
    height(height)
{
    floor = new Animation(display);
    falling = new PointAnimation(display, 4, 11);
}

TetrisGame::~TetrisGame()
{   
}

void TetrisGame::animate(const Keyboard::key_state_map_t &keys)
{
    int x_translate = 0;
    for( const auto &entry : keys) 
    {
        const int key = entry.first;
        const int presses = entry.second;
        if( key == L_JOYSTICK_PIN && presses>0 )
        {
            x_translate -= presses;
        }
        if( key == R_JOYSTICK_PIN && presses>0 )
        {
            x_translate += presses;
        }
    }
    // remove old pixels
    for( const auto &pixel : falling->getPixels())
    {
        display.delPixel(pixel.x, pixel.y);
    }
    if( falling->boundingBox().start_y == 0)
    {
        falling->setColor(100,200,50);
        for( const auto &pixel : falling->getPixels())
        {
            display.setPixel(pixel.x, pixel.y, pixel.color);
        }
    }
    else
    {
        falling->translate( x_translate, -1);
        falling->moveInside();
        for( const auto &pixel : falling->getPixels())
        {
            display.setPixel(pixel.x, pixel.y, pixel.color);
        }
    }
    display.show();
}

/**
 * checks whether the top animation is standing on the other
 * animation
 */
bool TetrisGame::isOnTop(Animation &top, Animation &bottom) 
{
    auto top_pixels = top.getPixels();
    auto bottom_pixels = bottom.getPixels();

    for (auto tpix : top_pixels)
    {
        for (auto bpix : bottom_pixels)
        {
            if( (tpix.x == bpix.x) && (tpix.y-1 == bpix.y) )
            {
                return true;
            }
        }
    }
    return false;
}


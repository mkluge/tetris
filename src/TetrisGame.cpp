
#include "TetrisGame.h"

TetrisGame::TetrisGame(LEDDisplay &display, int width, int height) :
    display(display),
    width(width),
    height(height)
{
    
}

TetrisGame::~TetrisGame()
{
    
}

void TetrisGame::animate(Keys &keys)
{
    
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


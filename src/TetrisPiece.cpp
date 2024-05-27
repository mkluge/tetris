#include <TetrisPiece.h>
#include <stdlib.h>
#include <Arduino.h>
#include "esp_random.h"

TetrisPiece::TetrisPiece(LEDDisplay &display) : Animation(display)
{
    int avail_pieces = pieceTemplates.size();
    variant = esp_random() % avail_pieces;
    // calculate offset_x and offset_y so that piece
    // starts at the top of the screen and in the middle
    auto bBox = boundingBox();
    offset_x = (display_width/2) - ((bBox.end_x - bBox.start_x) / 2); 
    offset_y = -bBox.end_y;
}

TetrisPiece::~TetrisPiece()
{
}

int TetrisPiece::removeFullLines()
{
    int removed_lines = 0;
    // from the bottom up remove all full lines
    for( int line = display_height-1; line>=0; line--)
    {
        // count pieces in all lines
        int piecesPerLine[display_height];
        for( const auto &pixel : internal_pixels )
        {
            auto x = offset_x + pixel.x;
            auto y = offset_y + pixel.y;

            if (x < 0 || x>=display_width || y<0 || y>=display_width)
            {
                // shall not happen, but ...
                continue;
            }
            piecesPerLine[y]++;
        }
        if(piecesPerLine[line]==(display_width-1))
        {
            removed_lines++;
            removeLine(line);
            // we need to check the same line again
            // FIXME
            line ++;
        }
    }
    return removed_lines;
}

/**
 * all lines bigger than the selected one is moved one up
 */
void TetrisPiece::removeLine(int line)
{
    std::list<Pixel> new_pixels;
    for (auto &pixel : internal_pixels)
    {
        auto real_y = offset_y + pixel.y;
        if (real_y < line)
        {
            new_pixels.push_back(pixel);
        }
        else if (real_y > line)
        {
            pixel.y--;
            new_pixels.push_back(pixel);
        }
    }
    internal_pixels = new_pixels;
}

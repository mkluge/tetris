#ifndef TETRISPIECE_H
#define TETRISPIECE_H

#include "esp_random.h"
#include <Animation.h>
#include <Arduino.h>
#include <Basics.h>
#include <stdlib.h>
#include <vector>

#pragma once

class TetrisPiece : public Animation {
public:
  TetrisPiece(LEDDisplay &display) : Animation(display) {
    int avail_pieces = pieceTemplates.size();
    int variant = esp_random() % avail_pieces;
    // add pixels
    addPixels(pieceTemplates[variant]);
    // calculate offset_x and offset_y so that piece
    // starts at the top of the screen and in the middle
    auto bBox = boundingBox();
    offset_x = (display_width / 2) - ((bBox.end_x - bBox.start_x) / 2);
    offset_y = display.height() - bBox.end_y;
  }

  ~TetrisPiece() {}

  int removeFullLines() {
    int removed_lines = 0;
    // from the bottom up remove all full lines
    int piecesPerLine[display_height];
    for (int line = 0; line < display_height; line++) {
      piecesPerLine[line] = 0;
    }
    for (const auto &pixel : internal_pixels) {
      auto x = offset_x + pixel.x;
      auto y = offset_y + pixel.y;
      if (x < 0 || x >= display_width || y < 0 || y >= display_width) {
        Serial.println("buggy pixel");
      }
      piecesPerLine[y]++;
    }
    for (int line = display_height - 1; line >= 0; line--) {
      Serial.printf("Line: %2d, Pieces: %1d\n", line, piecesPerLine[line]);
    }
    // removing starts from the top
    for (int line = display_height - 1; line >= 0; line--) {
      // count pieces in all lines
      if (piecesPerLine[line] == display_width) {
        removed_lines++;
        removeLine(line);
      }
    }
    return removed_lines;
  }

  /**
   * all lines bigger than the selected one is moved one done
   * on the screen
   */
  void removeLine(int line) {
    std::list<Pixel> new_pixels;
    for (auto &pixel : internal_pixels) {
      auto real_y = offset_y + pixel.y;
      if (real_y < line) {
        new_pixels.push_back(pixel);
      } else if (real_y > line) {
        pixel.y--;
        new_pixels.push_back(pixel);
      }
    }
    internal_pixels = new_pixels;
  }

private:
  const std::vector<PixelList> pieceTemplates = {
      // yellow rectangle
      {{-1, 0, TYELLOW}, {0, 0, TYELLOW}, {-1, -1, TYELLOW}, {0, -1, TYELLOW}},
      // blue bar
      {{0, 0, TBLUE}, {0, 1, TBLUE}, {0, 2, TBLUE}, {0, 3, TBLUE}},
      // red s
      {{-1, -1, TRED}, {0, -1, TRED}, {0, 0, TRED}, {1, 0, TRED}},
      // green z
      {{-1, 0, TGREEN}, {0, 0, TGREEN}, {0, -1, TGREEN}, {1, -1, TGREEN}},
      // orange l
      {{0, -1, TORANGE}, {-1, -1, TORANGE}, {-1, 0, TORANGE}, {-1, 1, TORANGE}},
      // pink j
      {{0, 1, TPINK}, {0, 0, TPINK}, {0, -1, TPINK}, {-1, -1, TPINK}},
      // brown t
      {{-1, 0, TBROWN}, {0, 0, TBROWN}, {1, 0, TBROWN}, {0, -1, TBROWN}},
  };
};

#endif
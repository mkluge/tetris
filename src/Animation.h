#ifndef ANIMATION_H
#define ANIMATION_H

#include <Arduino.h>
#include <Basics.h>
#include <LEDDisplay.h>
#include <esp_random.h>
#include <list>

#pragma once
class Animation {
public:
  enum Rotation {
    LEFT,
    RIGHT,
  };

  Animation(LEDDisplay &display) : display(display) {
    display_width = display.width();
    display_height = display.height();
    offset_x = 0;
    offset_y = 0;
  }

  /***
   * calculates if a animation can be rotated clockwise
   * without leaving the screen or crashing into the given
   * animation, leaves calculated new pixels in the internal
   * pixel cache, cache incomplete if function returns false
   */
  bool canRotate(Animation *mayCrashInto) {
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
    for (auto &pixel : internal_pixels) {
      int x, y;
      if (direction == RIGHT) {
        x = offset_x - pixel.y;
        y = offset_y + pixel.x;
      }
      if (direction == LEFT) {
        x = offset_x + pixel.y;
        y = offset_y - pixel.x;
      }
      if (pixelOutsideScreen(x, y)) {
        return false;
      }
      if (pixelInside(x, y, mayCrashInto)) {
        return false;
      }
      cache_pixels.push_back({x, y, pixel.color});
    }
    return true;
  }

  const bool pixelOutsideScreen(int x, int y) const {
    if (x < 0 || x >= display_width || y < 0 || y >= display_height) {
      return true;
    }
    return false;
  }

  const bool pixelInside(int x, int y, Animation *mayCrashInto) const {
    for (const auto &fpix : mayCrashInto->getPixels()) {
      if ((fpix.x == x) && (fpix.y == y)) {
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
  bool canTranslate(int x, int y, Animation *mayCrashInto) {
    for (auto &pixel : internal_pixels) {
      // new place for this pixel
      int pixelx = offset_x + pixel.x + x;
      int pixely = offset_y + pixel.y + y;
      // now check conditions
      if (pixelOutsideScreen(x, y)) {
        Serial.println(x);
        Serial.println(y);
        Serial.println("outside");
        return false;
      }
      if (pixelInside(x, y, mayCrashInto)) {
        Serial.println("crash");
        return false;
      }
      cache_pixels.push_back({x, y, pixel.color});
    }
    return true;
  }

  void setColor(unsigned short red, unsigned short green, unsigned short blue) {
    for (auto &pixel : internal_pixels) {
      pixel.color = {red, green, blue};
    }
  }

  void moveInside(Animation *mayCrashInto) {
    int move_x = 0;
    int move_y = 0;
    struct Rect bb = boundingBox();
    if (bb.start_x < 0) {
      move_x = -bb.start_x;
    } else if (bb.end_x >= display.width()) {
      move_x = -1 - (bb.end_x - display.width());
    }
    if (bb.start_y < 0) {
      move_y = -bb.start_y;
    } else if (bb.end_y >= display.height()) {
      move_y = -1 - (bb.end_y - display.height());
    }
    if (move_x != 0 || move_y != 0) {
      if (canTranslate(move_x, move_y, mayCrashInto)) {
        useCachePixels();
      }
    }
  }

  void useCachePixels() { internal_pixels = cache_pixels; }

  const Rect boundingBox() const {
    struct Rect retval = {display.width(), display.height(), 0, 0};
    for (const auto &pixel : internal_pixels) {
      int x = pixel.x;
      int y = pixel.y;
      if (x < retval.start_x) {
        retval.start_x = pixel.x;
      }
      if (y < retval.start_y) {
        retval.start_y = y;
      }
      if (x > retval.end_x) {
        retval.end_x = x;
      }
      if (y > retval.end_y) {
        retval.end_y = y;
      }
    }
    return retval;
  }

  void paint(const int color_variation = 0) const {
    for (auto &pixel : internal_pixels) {
      auto color = pixel.color;
      // variate colors up to color_variation for rgb
      if (color_variation != 0) {
        uint32_t rand_num = esp_random();
        // take 10 bits for each pixel
        int red_pixels = (rand_num & 255);
        int green_pixels = (rand_num >> 8) & 255;
        int blue_pixels = (rand_num >> 16) & 255;
        // so the pixels are always +- original value
        int range = color_variation << 1;
        int red_variation = (red_pixels % range) - color_variation;
        int green_variation = (green_pixels % range) - color_variation;
        int blue_variation = (blue_pixels % range) - color_variation;
        color = {
            (unsigned short)(minmax(0, 255, (int)(color.red + red_variation))),
            (unsigned short)(minmax(0, 255,
                                    (int)(color.green + green_variation))),
            (unsigned short)(minmax(0, 255,
                                    (int)(color.blue + blue_variation))),
        };
      }
      display.setPixel(pixel.x, pixel.y, color);
    }
  }

  void unpaint() const {
    for (auto &pixel : internal_pixels) {
      display.delPixel(pixel.x + offset_x, pixel.y + offset_y);
    }
  }

  void addPixels(const PixelList &toadd) {
    for (auto pixel : toadd) {
      internal_pixels.push_back(pixel);
    }
  }

  void removePixels(const PixelList &toremove) {
    for (const auto pixel : toremove) {
      internal_pixels.remove(pixel);
    }
  }

  const PixelList getPixels() const {
    PixelList retval;
    for (auto &pixel : internal_pixels) {
      retval.push_back({pixel.x + offset_x, pixel.y + offset_y, pixel.color});
    }
    return retval;
  }

  void clearPixels() { internal_pixels.clear(); }

protected:
  LEDDisplay &display;
  PixelList internal_pixels;
  PixelList cache_pixels;
  int offset_x;
  int offset_y;
  int display_width;
  int display_height;
};

#endif
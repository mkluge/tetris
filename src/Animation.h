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
    clear();
  }

  const bool pixelOutsideScreen(int x, int y) const {
    if ((offset_x + x) < 0 || (offset_x + x) >= display_width ||
        (offset_y + y) < 0 || (offset_y + y) >= display_height) {
      return true;
    }
    return false;
  }

  const bool pixelInsideOtherAnimation(int x, int y,
                                       Animation *mayCrashInto) const {
    for (const auto &fpix : mayCrashInto->getPixels()) {
      if ((fpix.x == (offset_x + x)) && (fpix.y == (offset_y + y))) {
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
      int pixelx = pixel.x + x;
      int pixely = pixel.y + y;
      // now check conditions
      if (pixelOutsideScreen(pixelx, pixely)) {
        //Serial.println(pixelx);
        //Serial.println(pixely);
        //Serial.println("outside");
        return false;
      }
      if (pixelInsideOtherAnimation(pixelx, pixely, mayCrashInto)) {
        //Serial.println("crash");
        return false;
      }
    }
    offset_x_cache = offset_x + x;
    offset_y_cache = offset_y + y;
    cached_pixels = internal_pixels;
    return true;
  }

  /***
   * calculates if a animation can be rotated clockwise
   * without leaving the screen or crashing into the given
   * animation, leaves calculated new pixels in the internal
   * pixel cache, cache incomplete if function returns false
   */
  bool canRotate(Animation *mayCrashInto) {
    cached_pixels.clear();
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
      int x = (-(2*pixel.y-center_y)+center_x) / 2;
      int y = pixel.x+(center_y-center_x)/2;
      //Serial.printf("ox: %d, oy: %d, rx: %d, ry: %d\n", pixel.x, pixel.y, x, y);
      if (pixelOutsideScreen(x, y)) {
        cached_pixels.clear();
        return false;
      }
      if (pixelInsideOtherAnimation(x, y, mayCrashInto)) {
        cached_pixels.clear();
        return false;
      }
      cached_pixels.push_back({x, y, pixel.color});
    }
    offset_x_cache = offset_x;
    offset_y_cache = offset_y;
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

  void useCachePixels() {
    internal_pixels = cached_pixels;
    offset_x = offset_x_cache;
    offset_y = offset_y_cache;
  }

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

  void paint(const int color_variation = 0) {
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
      display.setPixel(pixel.x + offset_x, pixel.y + offset_y, color);
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

  void clear() {
    offset_x = 0;
    offset_y = 0;
    offset_x_cache = 0;
    offset_y_cache = 0;
    center_x = 0;
    center_y = 0;
    cached_pixels.clear();
    internal_pixels.clear();
  }

protected:
  LEDDisplay &display;
  PixelList internal_pixels;
  PixelList cached_pixels;
  int offset_x;
  int offset_y;
  int offset_x_cache;
  int offset_y_cache;
  int center_x;
  int center_y;
  int display_width;
  int display_height;
};

#endif
#ifndef TETRISGAME_H
#define TETRISGAME_H

#include <GameInterface.h>
#include <Keyboard.h>
#include <LEDDisplay.h>
#include <TM1637Display.h>
#include <TetrisPiece.h>
#include <functional>
#include <list>

#pragma once

class TetrisGame : public GameInterface {
public:
  TetrisGame(LEDDisplay &display, int width, int height,
             TM1637Display &point_leds)
      : display(display), width(width), height(height), point_leds(point_leds) {
        ended = false;
  }

  ~TetrisGame() {}

  void start() {
    floor = new TetrisPiece(display);
    floor->clear();
    falling = new TetrisPiece(display);
    timer_interval = 700;
    points = 0;
    point_leds.showNumberDec(points);
    last_render_millis = millis();
    last_input_millis = millis();
  }

  void stop() {}

  void onKey(const Keyboard::key_state_map_t &keys) {
    int x_translate = 0;
    for (const auto &entry : keys) {
      const int key = entry.first;
      const int presses = entry.second;
      if (key == L_JOYSTICK_PIN && presses) {
        if (falling->canTranslate(-1, 0, floor)) {
          falling->unpaint();
          falling->useCachePixels();
          falling->paint();
        }
      }
      if (key == R_JOYSTICK_PIN && presses) {
        if (falling->canTranslate(1, 0, floor)) {
          falling->unpaint();
          falling->useCachePixels();
          falling->paint();
        }
      }
      if (key == D_JOYSTICK_PIN && presses) {
        // move element down as far as possible
        while (falling->canTranslate(0, -1, floor)) {
          falling->unpaint();
          falling->useCachePixels();
          falling->paint();
        }
      }
      if ((key == L_PUSH_PIN || key == R_PUSH_PIN || key == U_JOYSTICK_PIN) && presses) {
        if (falling->canRotate(floor)) {
          falling->unpaint();
          falling->useCachePixels();
          falling->paint();
        }
      }
    }
    display.show();
  }

  bool canHandleInput() {
    int millies = millis();
    if (millies - last_input_millis > 20) {
      last_input_millis = millies;
      return true;
    } else {
      return false;
    }
  }

  /**
   * animate is called as often as possible
   */
  void animate() {
    // get the current millis, get the diff and
    // figure out, if we need to do a step
    unsigned millies = millis();
    auto diff = millies - last_render_millis;
    if (diff >= timer_interval) {
      // OK, make a step
      // let element fall 1 step
      // remove old pixels
      falling->unpaint();
      // do we hit the floor or the ground
      bool canProceed = falling->canTranslate(0, -1, floor);
      if (!canProceed) {
        // check if game is over because highest
        // line is at top row
        for (const auto &pixel : falling->getPixels()) {
          if (pixel.y == (height - 1)) {
            // game finished
            display.clear();
            stop();
            start();
            ended = true;
            return;
          }
        }
        // merge falling into floor
        floor->unpaint();
        floor->addPixels(falling->getPixels());
        // check for complete rows
        // future: funny animation for full row removal
        // and remove complete rows, get points
        int numDeletedRows = floor->removeFullLines();
        points += numDeletedRows * 50;
        point_leds.showNumberDec(points);
        // draw new floor
        floor->paint();
        // create new falling piece
        falling = new TetrisPiece(display);
      } else {
        // we already checked whether there is
        // space below so just go one down
        falling->useCachePixels();
        falling->paint();
      }
      // store time of last action
      last_render_millis = millies;
      // decrease delay between steps
      // this increases game speed
      if (timer_interval > 100) {
        timer_interval--;
      }
      floor->paint();
      display.show();
    }
  }

  bool hasEnded() {
    return ended;
  }

private:
  LEDDisplay &display;
  int width;
  int height;
  TM1637Display &point_leds;
  int points;
  bool ended;
  TetrisPiece *falling;
  TetrisPiece *floor;
  unsigned long timer_interval;
  unsigned long last_render_millis;
  unsigned long last_input_millis;
};

void run_tetris() {
  TetrisGame tetris = TetrisGame(display, PIXELS_X, PIXELS_Y, l8_left);

  tetris.start();

  while (true) {
    // first handle keys, if any
    if (tetris.canHandleInput()) {
      auto keys = keyboard.toggled();
      if( keys.size() )
      {
        tetris.onKey(keys);
      }
    }
    
    // then call animation
    tetris.animate();
    if (tetris.hasEnded()) {
      renderBlood();
      return;
    }
  }
}

#endif
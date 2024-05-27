#ifndef TETRISGAME_H
#define TETRISGAME_H

#include <list>
#include <functional>
#include <TetrisPiece.h>
#include <LEDDisplay.h>
#include <Keyboard.h>
#include <GameInterface.h>
#include <TM1637Display.h>

#pragma once

class TetrisGame : public GameInterface
{
public:
    TetrisGame(LEDDisplay &display, int width, int height, TM1637Display& point_leds);
    ~TetrisGame();

    void start();
    void stop();
    void onKey(const Keyboard::key_state_map_t &keys);
    void animate();

private:
    bool isOnTop( const Animation *top, const Animation *bottom) const;
    Animation *createPiece();

private:
    LEDDisplay &display;
    int width;
    int height;
    TM1637Display& point_leds;
    int points;
    TetrisPiece *falling;
    TetrisPiece *floor;
    unsigned long timer_interval;
    unsigned long last_millis;
};

#endif
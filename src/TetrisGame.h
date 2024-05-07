#ifndef TETRISGAME_H
#define TETRISGAME_H

#include <list>
#include <functional>
#include <Animation.h>
#include <LEDDisplay.h>
#include <Keyboard.h>


#pragma once

class TetrisGame
{
public:
    TetrisGame(LEDDisplay &display, int width, int height);
    ~TetrisGame();

    void animate(const Keyboard::key_state_map_t &keys);
    bool isOnTop( Animation &top, Animation &bottom);

private:
    LEDDisplay &display;
    int width;
    int height;
    Animation *falling;
    Animation *floor;
};

#endif
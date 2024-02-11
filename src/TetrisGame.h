#ifndef TETRISGAME_H
#define TETRISGAME_H

#include <list>
#include <functional>
#include <Animation.h>
#include <LEDDisplay.h>

#pragma once

class TetrisGame
{
public:
    TetrisGame(LEDDisplay &display, int width, int height);
    ~TetrisGame();

    void animate(Keys &keys);
    bool isOnTop( Animation &top, Animation &bottom);

private:
    LEDDisplay &display;
    int width;
    int height;
    std::list<std::reference_wrapper<Animation>> animations;
};

#endif
#ifndef TETRISGAME_H
#define TETRISGAME_H

#include <list>
#include <functional>
#include <Animation.h>
#include <LEDDisplay.h>
#include <Keyboard.h>
#include <GameInterface.h>


#pragma once

class TetrisGame
{
public:
    TetrisGame(LEDDisplay &display, int width, int height);
    ~TetrisGame();

    static void start(const Scheduler &scheduler);
    static void stop(const Scheduler &scheduler);
    static void onKey(const Keyboard::key_state_map_t &keys);
    static void animate();

private:
    bool isOnTop( Animation &top, Animation &bottom);

private:
    inline static LEDDisplay &display;
    inline static int width;
    inline static int height;
    inline static Animation *falling;
    inline static Animation *floor;
};

#endif
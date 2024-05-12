#ifndef TETRISGAME_H
#define TETRISGAME_H

#include <list>
#include <functional>
#include <Animation.h>
#include <LEDDisplay.h>
#include <Keyboard.h>
#include <GameInterface.h>


#pragma once

class TetrisGame : public GameInterface
{
public:
    TetrisGame(LEDDisplay &display, int width, int height);
    ~TetrisGame();

    void start(const Scheduler &scheduler);
    void stop(const Scheduler &scheduler);
    void onKey(const Keyboard::key_state_map_t &keys);
    static void animate();

private:
    bool isOnTop( Animation &top, Animation &bottom);

private:
    static LEDDisplay &display;
    int width;
    int height;
    static Animation *falling;
    Animation *floor;
};

#endif
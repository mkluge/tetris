#ifndef GAMEINTERFACE_H
#define GAMEINTERFACE_H

#include <LEDDisplay.h>
#include <Keyboard.h>
#include <TaskSchedulerDeclarations.h>

#pragma once

class GameInterface
{
public:
    void start();
    void stop();
    void onKey(const Keyboard::key_state_map_t &keys);
};

#endif
#ifndef GAMEINTERFACE_H
#define GAMEINTERFACE_H

#include <LEDDisplay.h>
#include <Keyboard.h>
#include <TaskSchedulerDeclarations.h>

#pragma once

class GameInterface
{
public:
    static void start(const Scheduler &scheduler);
    static void stop(const Scheduler &scheduler);
    static void onKey(const Keyboard::key_state_map_t &keys);
};

#endif
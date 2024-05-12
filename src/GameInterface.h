#ifndef GAMEINTERFACE_H
#define GAMEINTERFACE_H

#include <LEDDisplay.h>
#include <Keyboard.h>
#include <TaskSchedulerDeclarations.h>

#pragma once

class GameInterface
{
public:
    virtual void start(const Scheduler &scheduler) = 0;
    virtual void stop(const Scheduler &scheduler) = 0;
    virtual void onKey(const Keyboard::key_state_map_t &keys) = 0;
};

#endif
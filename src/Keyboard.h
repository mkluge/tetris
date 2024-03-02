#ifndef KEYBOARD_H
#define KEYBOARD_H

#pragma once
#include <list>
#include <map>
#include <Arduino.h>

class Keyboard
{
public:
    typedef std::map<int, int> key_map_t;
    typedef std::map<int, bool> key_state_map_t;
    Keyboard();
    ~Keyboard();

    static void IRAM_ATTR isr();
    void addKey(int id, int pin);
    key_state_map_t toggled();

private:
    static key_map_t keys;
    static key_state_map_t old_state;
    static key_state_map_t toggled_keys;
};

#endif
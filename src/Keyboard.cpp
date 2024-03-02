#include "Keyboard.h"

Keyboard::Keyboard()
{
}

Keyboard::~Keyboard()
{

}

void IRAM_ATTR Keyboard::isr()
{
    for( auto const &kv: keys)
    {
        bool pressed = digitalRead(kv.second);
        if (pressed != old_state[kv.first])    
        {
            toggled_keys[kv.first]=pressed;
        }                                      
    }
}

void Keyboard::addKey(int id, int pin)
{
    pinMode(pin, INPUT_PULLUP);
    keys[id]=pin;
    old_state[id]=false;
    attachInterrupt(pin, isr, FALLING);
}

Keyboard::key_state_map_t Keyboard::toggled()
{
    // run again to check state
    for (auto const &kv : keys)
    {
        bool pressed = digitalRead(kv.second);
        if (pressed != old_state[kv.first])
        {
            toggled_keys[kv.first] = pressed;
        }
    }
    // copy current state
    for (auto const &kv : keys)
    {
        old_state[kv.first] = digitalRead(kv.second);
    }
    auto retval = toggled_keys;
    toggled_keys.clear();
    return retval;
}


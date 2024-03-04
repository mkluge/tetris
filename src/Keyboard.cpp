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
            // we are running low active
            toggled_keys[kv.first]=!pressed;
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
    noInterrupts();
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
    // negate all values as we are running low active
    key_state_map_t retval = toggled_keys;
    toggled_keys.clear();
    interrupts();
    return retval;
}


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
            // only one event per key
            for( auto const &stored: toggled_keys)
            {
                if(stored.first==kv.first)
                {
                    return;
                }
            }
            // we are running low active
            toggled_keys[kv.first]=!pressed;
        }                                      
    }
}

void Keyboard::addKey(int id, int pin)
{
    pinMode(pin, INPUT_PULLUP);
//    noInterrupts();
    keys[id] = pin;
    old_state[id]=false;
    attachInterrupt(pin, isr, FALLING);
//    interrupts();
}

Keyboard::key_state_map_t Keyboard::toggled()
{
//    noInterrupts();
    // run again to check state and report any
    // differences
    for (auto const &kv : keys)
    {
        bool pressed = digitalRead(kv.second);
        if (pressed != old_state[kv.first])
        {
            // only one event per key
            bool found = false;
            for (auto const &stored : toggled_keys)
            {
                if (stored.first == kv.first)
                {
                    found = true;
                    break;
                }
            }
            if(!found)
            {
                toggled_keys[kv.first] = pressed;
            }
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
//    interrupts();
    return retval;
}


#include "Keyboard.h"

Keyboard::Keyboard()
{
}

Keyboard::~Keyboard()
{

}

// so what we do here is counting how often
// a key was hit (and maybe released)
void IRAM_ATTR Keyboard::isr()
{
    for( auto const &kv: keys)
    {
        // read and negate as we are running low active
        bool pressed = !digitalRead(kv.second);
        if (pressed != old_state[kv.first])    
        {
            // if it was a release, then at least
            // store the 0
            if( !pressed )
            {
                if( toggled_keys.find(kv.first) != toggled_keys.end() )
                {
                    toggled_keys[kv.first]=0;
                }
            } else {
            // if it was a press, store a 1 or increase
                if( toggled_keys.find(kv.first) != toggled_keys.end() )
                {
                    toggled_keys[kv.first]=1;
                } else {
                    toggled_keys[kv.first]+=1;
                }
            }
            old_state[kv.first] = pressed;
        }                                      
    }
}

/** @brief add a pin to the list of monitored pins
*/
void Keyboard::addKey(int id, int pin)
{
    pinMode(pin, INPUT_PULLUP);
//    noInterrupts();
    keys[id] = pin;
    // store initial "LOW" for each key
    old_state[id]=false;
    attachInterrupt(pin, isr, FALLING);
//    interrupts();
}


/** @brief returns a list of pressed keys
 *  @return a list of keys pressed as a map 
 *          0 as value means: key was only released
 *          1 or higher: how often it has been pressed
 *          if a key is not in the list this means it
 *          has neither pressed or released
*/       
Keyboard::key_state_map_t Keyboard::toggled()
{
//    noInterrupts();
    // copy current state
    for (auto const &kv : keys)
    {
        old_state[kv.first] = digitalRead(kv.second);
    }
    key_state_map_t retval = toggled_keys;
    toggled_keys.clear();
//    interrupts();
    return retval;
}


#ifndef GAMEEVENTS_H
#define GAMEEVENTS_H

#include <Arduino.h>
#include <Keyboard.h>

extern Keyboard keyboard;

enum class GameEventType {
    Timer,
    Key,
};

struct GameEvent {
    GameEventType type;
    Keyboard::key_state_map_t keys;
};

class GameEventLoop {
public:
    GameEventLoop(unsigned long timer_interval_ms)
        : timer_interval_ms(timer_interval_ms), last_timer_millis(millis())
    {
    }

    void reset()
    {
        last_timer_millis = millis();
        keyboard.toggled();
    }

    void setTimerInterval(unsigned long interval_ms)
    {
        timer_interval_ms = interval_ms;
    }

    bool next(GameEvent &event)
    {
        Keyboard::key_state_map_t keys = keyboard.toggled();
        if (keys.size()) {
            event.type = GameEventType::Key;
            event.keys = keys;
            return true;
        }

        unsigned long now = millis();
        if (now - last_timer_millis >= timer_interval_ms) {
            last_timer_millis += timer_interval_ms;
            event.type = GameEventType::Timer;
            event.keys.clear();
            return true;
        }

        yield();
        return false;
    }

private:
    unsigned long timer_interval_ms;
    unsigned long last_timer_millis;
};

#endif

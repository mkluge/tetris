#ifndef PONG1D_H
#define PONG1D_H

#include <Arduino.h>
#include <LEDDisplay.h>
#include <array>
#include <map>
#include <FastTrig.h>
#include <math.h>
#pragma once

class Pong1D {
private:
    int paddle_size = 3;
    int paddle_pos = 5;
    int paddle_direction = 1;

    // to be used
    int score_left = 0;
    int score_right = 0;

    // field size including buttons (2 buttons + 22 pixels on screen)
    const int field_size = 24;

    bool running = true;

    LEDDisplay &display;

public:
    Pong1D(LEDDisplay &display) : display(display) {}

    // move one step
    void move_step() {
        if (!running) {return;}
        
        int paddle_right_end = paddle_pos + paddle_size - 1;

        if (paddle_right_end < 0) {
            // TODO: end game
            paddle_direction *= -1;
        }

        if (paddle_pos > field_size) {
            // TODO: end game
            paddle_direction *= -1;
        }

        paddle_pos += paddle_direction;
    }

    void paint() {
        int x_by_field_pos[] = {-1, 0, 0, 0, 0, 0, 1, 1, 1, 2, 2, 3, 4, 5, 5, 6, 6, 6, 7, 7, 7, 7, 7, -1};
        int y_by_field_pos[] = {-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, -1};

        display.clear();

        for (int i = 0; i < paddle_size; i++) {
            int painting_field_pos = paddle_pos + i;
            if ((painting_field_pos >= 1) && (painting_field_pos < (field_size - 1))) {
                int x = x_by_field_pos[painting_field_pos];
                int y = y_by_field_pos[painting_field_pos];

                if ((x >= 0) || (y >= 0)) {
                    display.setPixel(x, y, {255, 255, 255});
                }
            }
        }

        display.show();
    }
};
#endif

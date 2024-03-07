#ifndef STEINSCHEREPAPIER_H
#define STEINSCHEREPAPIER_H

#include <Arduino.h>
#include <LEDDisplay.h>
#include <array>
#include <map>

#pragma once

template <int size_x, int size_y>
class SteinScherePapier
{
public:
    SteinScherePapier(LEDDisplay &display) : display(display)
    {
    }

    ~SteinScherePapier()
    {
    }

    void init()
    {
        for (auto x = 0; x < size_x; x++)
        {
            for (auto y = 0; y < size_y; y++)
            {
                state[x][y] = random() % 3;
            }
        }
        paint();
    };

    void animate()
    {
        // rules: 0 beats 1, 1 beats 2, 2 beats 0
        // if at least two neigbours beat me, I change colors
        for (auto x = 0; x < size_x; x++)
        {
            for (auto y = 0; y < size_y; y++)
            {
                auto my_state = state[x][y];
                auto win_state = (my_state + 2) % 3;
                std::map<short,int> neig_count = {
                    {0,0}, {1,0}, {2,0},
                };
                // bottom neighbor
                if (y > 0)
                {
                    auto col = state[x][y - 1];
                    neig_count[col] = neig_count[col] + 1;
                }
                // top neighbor
                if (y < (size_y-1))
                {
                    auto col = state[x][y + 1];
                    neig_count[col] = neig_count[col] + 1;
                }
                // left neighbor
                if (x > 0)
                {
                    auto col = state[x-1][y];
                    neig_count[col] = neig_count[col] + 1;
                }
                // right neighbor
                if (x < (size_x-1))
                {
                    auto col = state[x+1][y];
                    neig_count[col] = neig_count[col] + 1;
                }
                // has win_state a count of at least 2?
                if( neig_count[win_state] >=2 )
                {
                    new_state[x][y] = win_state;
                }
                else
                {
                    new_state[x][y] = state[x][y];
                }
                // if not, old state just stays
            }
        }
        state = new_state;
        paint();
    }

    void paint()
    {
        std::map<short, RGB> colormap{
            {0, {155, 155, 155}},
            {1, {220, 83, 0}},
            {2, {30, 75, 150}},
        };
        for (auto x = 0; x < size_x; x++)
        {
            for (auto y = 0; y < size_y; y++)
            {
                auto color = colormap[state[x][y]];
                display.setPixel(x, y, color);
            }
        }
        display.show();
    }

private:
    LEDDisplay &display;
    std::array<std::array<short, size_y>, size_x> state;
    std::array<std::array<short, size_y>, size_x> new_state;
};

#endif
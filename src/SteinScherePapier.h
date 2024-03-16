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

    int winNeighbor9(int x, int y)
    {
        auto my_state = state[x][y];
        auto win_state = (my_state + 2) % 3;
        auto num_wins = sumNeighborState( x, y, win_state);
        if( num_wins>=3 )
        {
            return win_state;
        }
        return my_state;
    }

    int sumNeighborState( int start_x, int start_y, short lookup_state) const
    {
        int hits = 0;
        for( auto x=max(start_x-1,0); x<=min(start_x+1,size_x-1); x++ )
        {
            for (auto y = max(start_y-1,0); y <=min(start_y+1,size_y-1); y++)
            {
                if (state[x][y]==lookup_state)
                {
                    hits++;
                }
            }
        }
        return hits;
    }

    int bestNeighbor(int start_x, int start_y) const
    {
        int hits[3] = {0,0,0};
        for (auto x = max(start_x - 1, 0); x <= min(start_x + 1, size_x - 1); x++)
        {
            for (auto y = max(start_y - 1, 0); y <= min(start_y + 1, size_y - 1); y++)
            {
                hits[state[x][y]]++;
            }
        }
        if (hits[0] > hits[1] && hits[0] > hits[2])
        {
            return 0;
        }
        if (hits[1] > hits[2])
        {
            return 1;
        }
        return 2;
    }

    void animate()
    {
        // rules: 0 beats 1, 1 beats 2, 2 beats 0
        // if at least two neigbours beat me, I change colors
        // start with the default: all states are preserved
        new_state = state;
        for (auto x = 0; x < size_x; x++)
        {
            for (auto y = 0; y < size_y; y++)
            {
                new_state[x][y]=winNeighbor9(x,y);
            }
        }
        state = new_state;
        paint();
    }

    void paint()
    {
        std::map<short, RGB> colormap{
            {0, {155, 155, 155}},
            {1, {105, 42, 53}},
//            {1, {220, 83, 0}},
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
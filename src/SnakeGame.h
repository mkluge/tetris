#ifndef SNAKE_H
#define SNAKE_H

#include <Arduino.h>
#include <LEDDisplay.h>
#include <SnakePixel.h>
#pragma once


template <int size_x, int size_y>
class SnakeGame
{
    enum SNAKE_DIRECTION {
        LEFT,
        RIGHT,
        UP,
        DOWN
    };
    public:
        SnakeGame(LEDDisplay &display) : display(display)
        {

        }
        ~SnakeGame() {}
        void init() {
            // Initialization stuff here
            this->currentSnakeLength = 3;
            this->currentSnakeDirection = SnakeGame::SNAKE_DIRECTION::RIGHT;
            // TODO: do a memset(0) on the mySnake array...
            this->mySnake[0][0] = 1;
            this->mySnake[0][1] = size_y / 2;
            this->mySnake[1][0] = 0;
            this->mySnake[1][1] = size_y / 2;
        }
        int* calculateSnakeDirection() {
            int nextSnakePos[2] = {
                this->mySnake[0][0],
                this->mySnake[0][1]
            };
            // TODO: implement collision detection
            switch(this->currentSnakeDirection) {
                case SNAKE_DIRECTION::LEFT:
                    nextSnakePos[0]--;
                    // Out of Bounds: Left side
                    if(0 > nextSnakePos[1])
                        nextSnakePos[1] = size_x - 1;
                case SNAKE_DIRECTION::RIGHT:
                    nextSnakePos[0]++;
                    // Out of Bounds: Right side
                    if(size_x >= nextSnakePos[0])
                        nextSnakePos[0] = 0;
                case SNAKE_DIRECTION::UP:
                    nextSnakePos[1]++;
                    // Out of Bounds: Top
                    if(size_y >= nextSnakePos[1])
                    nextSnakePos[1] = 0;
                case SNAKE_DIRECTION::DOWN:
                    nextSnakePos[1]--;
                    // Out of Bounds: Bottom
                    if(0 > nextSnakePos[1])
                      nextSnakePos[1] = size_y - 1;
                    break;
            }
            return nextSnakePos;
        }
        void moveSnake(int nextSnakePos[]) {
            // Reset last Pixel
            this->snakeMatrix[
                this->mySnake[this->currentSnakeLength - 1][0],
                this->mySnake[this->currentSnakeLength - 1][1]].setPixel(
                    SnakePixel::SNAKE_OBJECT_TYPE::EMPTY,
                    0.0,
                    0.0);
            // Move other pieces of the Snake a pixel further
            for(int i = 0; i < this->currentSnakeLength - 1; i++)
            {
                this->mySnake[i + 1][0] = this->mySnake[i][0];
                this->mySnake[i + 1][1] = this->mySnake[i][1];
                this->snakeMatrix[
                    this->mySnake[i][0],
                    this->mySnake[i][1]].age();
            }
            this->mySnake[0][0] = nextSnakePos[0];
            this->mySnake[0][1] = nextSnakePos[1];
            this->snakeMatrix[
                this->mySnake[0][0],
                this->mySnake[0][1]
            ].setPixel(
                SnakePixel::SNAKE_OBJECT_TYPE::SNAKE,
                0.0,
                0.0);
        }
        void snakeGameEngine() {
            // TODO: Do magic stuff here (i.e. calculate frame)
            if(0 == (this->counter % 6)) // be a little slower
            {
                int* nextSnakePos = this->calculateSnakeDirection();
                this->moveSnake(nextSnakePos);
            }

            // TODO: fill snakeMatrix (and it's color)
            this->counter++;
        }
        void paint() {
            this->snakeGameEngine();

            int x = 0,
                y = 0;
            for(int y_index = 0; y_index < size_y; y_index++)
              for(int x_index = 0; x_index < size_x; x_index++)
                display.setPixel(x, y, this->snakeMatrix[x_index, y_index]->getColor());
            display.show();
        }
    private:
        LEDDisplay &display;
        SnakePixel snakeMatrix[size_x][size_y];
        static const int MAX_SNAKE_LENGTH = 100;
        int currentSnakeLength = 0;
        SnakeGame::SNAKE_DIRECTION currentSnakeDirection = SnakeGame::SNAKE_DIRECTION::RIGHT;
        int mySnake[MAX_SNAKE_LENGTH][2];
        int counter = 0;
};

#endif

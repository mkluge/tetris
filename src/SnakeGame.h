#ifndef SNAKE_H
#define SNAKE_H

#include <Arduino.h>
//#include <HardwareSerial.h>
#include <LEDDisplay.h>
#include <SnakePixel.h>
#pragma once

struct Position {
    int x;
    int y;
};

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
            for(int y_index = 0; y_index < size_y; y_index++)
                for(int x_index = 0; x_index < size_x; x_index++)
                    this->snakeMatrix[x_index][y_index].reset();
            for(int i = 0; i < MAX_SNAKE_LENGTH; i++)
            {
                this->mySnake[i][0] = 0;
                this->mySnake[i][1] = 0;
            }


            this->currentSnakeLength = 2;
            this->currentSnakeDirection = SnakeGame::SNAKE_DIRECTION::RIGHT;

            this->mySnake[0][0] = 1;
            this->mySnake[0][1] = size_y / 2;
            this->mySnake[1][0] = 0;
            this->mySnake[1][1] = size_y / 2;
            //Serial.begin(9600);
            //Serial.println("Hello World");
        }
        Position calculateSnakeDirection() {
            Position nextSnakePos = {
                this->mySnake[0][0],
                this->mySnake[0][1]
            };
            // TODO: implement collision detection
            switch(this->currentSnakeDirection) {
                case SNAKE_DIRECTION::LEFT:
                    nextSnakePos.x--;
                    // Out of Bounds: Left side
                    if(0 > nextSnakePos.y)
                        nextSnakePos.y = size_x - 1;
                case SNAKE_DIRECTION::RIGHT:
                    nextSnakePos.x++;
                    // Out of Bounds: Right side
                    if(size_x >= nextSnakePos.x)
                        nextSnakePos.x = 0;
                case SNAKE_DIRECTION::UP:
                    nextSnakePos.y++;
                    // Out of Bounds: Top
                    if(size_y >= nextSnakePos.y)
                    nextSnakePos.y = 0;
                case SNAKE_DIRECTION::DOWN:
                    nextSnakePos.y--;
                    // Out of Bounds: Bottom
                    if(0 > nextSnakePos.y)
                      nextSnakePos.y = size_y - 1;
                    break;
            }
            return nextSnakePos;
        }
        void moveSnake(Position nextSnakePos) {
            // Reset last Pixel
            this->snakeMatrix[this->mySnake[this->currentSnakeLength - 1][0]]
                [this->mySnake[this->currentSnakeLength - 1][1]].reset();

            // Move other pieces of the Snake a pixel further
            for(int i = 0; i < this->currentSnakeLength - 1; i++)
            {
                this->mySnake[i + 1][0] = this->mySnake[i][0];
                this->mySnake[i + 1][1] = this->mySnake[i][1];
                this->snakeMatrix[this->mySnake[i][0]]
                    [this->mySnake[i][1]].age();
            }

            this->mySnake[0][0] = nextSnakePos.x;
            this->mySnake[0][1] = nextSnakePos.y;

            this->snakeMatrix[this->mySnake[0][0]]
                [this->mySnake[0][1]].setPixel(
                SnakePixel::SNAKE_OBJECT_TYPE::SNAKE,
                0.0,
                0.0);
        }
        void snakeGameEngine() {
            // TODO: Do magic stuff here (i.e. calculate frame)
            if(0 == (this->counter % 6)) // be a little slower
            {
                Position nextSnakePos = this->calculateSnakeDirection();
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
                display.setPixel(x, y, this->snakeMatrix[x_index][y_index].getColor());
            display.show();
            //Serial.println("Lars.");
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

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
            // DEBUG:
            this->snakeMatrix[size_x - 1][size_y - 2].setPixel(
                SnakePixel::SNAKE_OBJECT_TYPE::WALL,
                0.0,
                0.0
            );
            for(int i = 0; i < MAX_SNAKE_LENGTH; i++)
            {
                this->mySnake[i] = {0, 0};
            }


            this->currentSnakeLength = 2;
            this->currentSnakeDirection = SnakeGame::SNAKE_DIRECTION::RIGHT;

            this->mySnake[0] = {1, size_y / 2 - 1};
            this->mySnake[1] = {0, size_y / 2 - 1};
            //Serial.begin(9600);
            //Serial.println("Hello World");
        }
        Position calculateSnakeDirection() {
            Position nextSnakePos = {
                this->mySnake[0].x,
                this->mySnake[0].y
            };
            // TODO: implement collision detection
            switch(this->currentSnakeDirection) {
                case SNAKE_DIRECTION::LEFT:
                    nextSnakePos.x--;
                    // Out of Bounds: Left side
                    if(0 > nextSnakePos.y)
                        nextSnakePos.y = size_x - 1;
                    break;
                case SNAKE_DIRECTION::RIGHT:
                    nextSnakePos.x++;
                    // Out of Bounds: Right side
                    if(size_x <= nextSnakePos.x)
                        nextSnakePos.x = 0;
                    break;
                case SNAKE_DIRECTION::UP:
                    nextSnakePos.y++;
                    // Out of Bounds: Top
                    if(size_y <= nextSnakePos.y)
                        nextSnakePos.y = 0;
                    break;
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
            this->snakeMatrix[this->mySnake[this->currentSnakeLength - 1].x]
                [this->mySnake[this->currentSnakeLength - 1].y].reset();

            // Move other pieces of the Snake a pixel further
            for(int i = 0; i < (this->currentSnakeLength - 1); i++)
            {
                this->mySnake[i + 1].x = this->mySnake[i].x;
                this->mySnake[i + 1].y = this->mySnake[i].y;
                this->snakeMatrix[this->mySnake[i + 1].x]
                    [this->mySnake[i + 1].y].age();
            }

            // Set the beginning of the Snake
            this->mySnake[0] = nextSnakePos;
            this->snakeMatrix[this->mySnake[0].x]
                [this->mySnake[0].y].setPixel(
                SnakePixel::SNAKE_OBJECT_TYPE::SNAKE,
                0.0,
                0.0);
        }
        bool snakeGameEngine() {
            // keyboard events
            for( const auto &key: keyboard.toggled()) {
                if (key.first == 18 && key.second && this->currentSnakeDirection != SnakeGame::SNAKE_DIRECTION::RIGHT) {
                    this->currentSnakeDirection = SnakeGame::SNAKE_DIRECTION::LEFT;
                }
                if (key.first == 21 && key.second && this->currentSnakeDirection != SnakeGame::SNAKE_DIRECTION::LEFT) {
                    this->currentSnakeDirection = SnakeGame::SNAKE_DIRECTION::RIGHT;
                }
                if (key.first == 19 && key.second && this->currentSnakeDirection != SnakeGame::SNAKE_DIRECTION::DOWN) {
                    this->currentSnakeDirection = SnakeGame::SNAKE_DIRECTION::UP;
                }
                if (key.first == 3 && key.second && this->currentSnakeDirection != SnakeGame::SNAKE_DIRECTION::UP) {
                    this->currentSnakeDirection = SnakeGame::SNAKE_DIRECTION::DOWN;
                }
            }

            // snake simulation
            if(0 == (this->counter % 6)) // be a little slower
            {
                Position nextSnakePos = this->calculateSnakeDirection();
                this->moveSnake(nextSnakePos);
            }

            // TODO: fill snakeMatrix (and it's color)
            this->counter++;
	    return false;
        }
        void paint() {
            for(int y_index = 0; y_index < size_y; y_index++)
              for(int x_index = 0; x_index < size_x; x_index++)
                display.setPixel(x_index, y_index, this->snakeMatrix[x_index][y_index].getColor());
                //display.setPixel(x_index, y_index, {192, 64, 64});
            display.show();
            //Serial.println("Lars.");
        }
    private:
        LEDDisplay &display;
        SnakePixel snakeMatrix[size_x][size_y];
        static const int MAX_SNAKE_LENGTH = 100;
        int currentSnakeLength = 0;
        SnakeGame::SNAKE_DIRECTION currentSnakeDirection = SnakeGame::SNAKE_DIRECTION::RIGHT;
        Position mySnake[MAX_SNAKE_LENGTH];
        int counter = 0;
};

// Lars disapproves of this coding style:
void run_snake() {
    // initialization
    int idle = 0;

    SnakeGame<8, 12> game(display);
    game.init();

    // main loop
    while (true) {
        int last_millis = millis();

        // game logic
	if (game.snakeGameEngine()) {
		return; // graceful exit
	}

        // game render
        game.paint();

        // busy waiting loop until next frame
        while (millis() - last_millis < 30) {
            // busy spin loop until frame time is over
        }

    }
}

#endif

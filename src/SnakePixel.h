#ifndef SNAKE_PIXEL_H
#define SNAKE_PIXEL_H

#include <LEDDisplay.h>

class SnakePixel {
   public:
        enum SNAKE_OBJECT_TYPE
        {
            EMPTY,
            SNAKE,
            FOOD,
            WALL
        };
        SnakePixel()
        {
            this->lastPixelColor.red = 0;
            this->lastPixelColor.green = 0;
            this->lastPixelColor.blue = 0;
            this->pixelAge = 0.0;
            this->modifier = 0.0;
        }
       void setPixel(SnakePixel::SNAKE_OBJECT_TYPE snakeType, double pixelAge, double modifier)
       {
            this->myType = snakeType;
            this->pixelAge = pixelAge;
            this->modifier = modifier;
       }
       void age()
       {
            this->pixelAge++;
       }
       RGB getColor()
       {

            switch(this->myType)
            {
                case SnakePixel::SNAKE_OBJECT_TYPE::SNAKE:
                    this->lastPixelColor.red = 0;
                    this->lastPixelColor.green = 255;
                    this->lastPixelColor.blue = 0;
                    break;
                case SnakePixel::SNAKE_OBJECT_TYPE::FOOD:
                    this->lastPixelColor.red = 176;
                    this->lastPixelColor.green = 176;
                    this->lastPixelColor.blue = 0;
                    break;
                case SnakePixel::SNAKE_OBJECT_TYPE::WALL:
                    this->lastPixelColor.red = 0;
                    this->lastPixelColor.green = 0;
                    this->lastPixelColor.blue = 255;
                    break;
            }
            // TODO: Also factor in pixelAge and modifier
            return this->lastPixelColor;
       }
    private:
        SnakePixel::SNAKE_OBJECT_TYPE myType;
        RGB lastPixelColor;

        double pixelAge;
        double modifier;
};

#endif

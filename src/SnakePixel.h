#ifndef SNAKE_PIXEL_H
#define SNAKE_PIXEL_H

#include <LEDDisplay.h>
#include <math.h>

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
            this->reset();
        }
        void reset()
        {
            this->myType = SnakePixel::SNAKE_OBJECT_TYPE::EMPTY;
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
            this->pixelAge += 1.0;
       }
       SNAKE_OBJECT_TYPE getType()
       {
            return this->myType;
       }
       RGB getColor()
       {

            switch(this->myType)
            {
                case SNAKE:
                    this->lastPixelColor.red = 0;
                    this->lastPixelColor.green = 127;
                    // APPLY age of snake elements
                    this->lastPixelColor.green += pow(0.99, this->pixelAge) * 127;
                    this->lastPixelColor.blue = 0;
                    break;
                case FOOD:
                    this->lastPixelColor.red = 176;
                    this->lastPixelColor.green = random(176);
                    this->lastPixelColor.blue = 0;
                    break;
                case WALL:
                    this->lastPixelColor.red = 0;
                    this->lastPixelColor.green = 0;
                    this->lastPixelColor.blue = 255;
                    break;
                case EMPTY:
                    this->lastPixelColor.red = 0;
                    this->lastPixelColor.green = 0;
                    this->lastPixelColor.blue = 0;
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

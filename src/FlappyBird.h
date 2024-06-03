#ifndef FLAPPYBIRD_H
#define FLAPPYBIRD_H

#include <LEDDisplay.h>
#include <Keyboard.h>
#include <TM1637Display.h>
#include <vector>

#pragma once

class Bird
{
private:
  RGB color = {255, 255, 255};
  int px = 1;
  int py = 1;

public:
  int getx() { return px; }
  int gety() { return py; }

  void show(LEDDisplay &display)
  {
    display.setPixel(px, py, color);
    display.show();
  }

  void jump(LEDDisplay &display)
  {
    // do stuff
  }
};

class ObstacleMap
{
private:
  const char *image[76] = {
      " .            .                    .                   .                   ",
      " .            .      .      .      .                   .                   ",
      " .      .     .      .      .      .     .             .      .            ",
      " .      .     .      .      .      .     .      .      .      .     .      ",
      " .      .     .      .      .      .     .      .      .      .     .      "};
  RGB color;
  int pos = 0;
  int pos2 = 0;
  int sz = strlen(image[0]);

public:
  void setColor(char c)
  {
    if (c == '.')
    {
      color = {255, 255, 255};
    }
    else
    {
      color = {0, 0, 0};
    }
  }

  void move(LEDDisplay &display)
  {
    for (int i = 0; i < 9; i++)
    {
      for (int j = 0; j < 8; j++)
      {
        setColor(image[i][(pos / 4 + j) % sz]);
        display.setPixel(j, 11 - i, color);
      }
    }
    display.show();

    pos = (pos + 1) % (4 * sz);
    pos2++;
  }
};

int FlappyBird(LEDDisplay &display)
{
  Bird bird;
  bird.show(display);

  ObstacleMap obs;

  // set ground
  for (int x = 0; x > 8; ++x)
  {
    display.setPixel(x, 0, RGB{0, 255, 0});
  }
  display.show();

  while (true)
  {
    obs.move(display);
  }
}

#endif

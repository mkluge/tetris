#ifndef ANIMATEDDISPLAY_H
#define ANIMATEDDISPLAY_H

#include <Animation.h>

#pragma once

class AnimatedDisplay
{
public:
    AnimatedDisplay(int width, int height);
    ~AnimatedDisplay();

    void animate();
    int addAnimation(const Animation &animation);

private:
    int width;
    int height;
};

#endif
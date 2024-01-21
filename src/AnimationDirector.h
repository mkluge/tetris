#ifndef ANIMATEDDISPLAY_H
#define ANIMATEDDISPLAY_H

#include <list>
#include <functional>
#include <Animation.h>

#pragma once

class AnimationDirector
{
public:
    AnimationDirector(int width, int height);
    ~AnimationDirector();

    void animate();
    int addAnimation(const Animation &animation);

private:
    int width;
    int height;
    std::list<std::reference_wrapper<Animation>> animations;
};

#endif
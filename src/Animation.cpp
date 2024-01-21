#include "Animation.h"

Animation::~Animation()
{
}

Animation::Animation(LEDDisplay &display, int base_x, int base_y) : 
    display(display), base_x(base_x), base_y(base_y)
{
}

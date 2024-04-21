#include <Basics.h>
#include <math.h>

bool operator==(const Pixel &lhs, const Pixel &rhs)
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

RGB ZeroOneToRainbow(double f)
{
    double a = (1 - f) / (double)0.2;
    double X = floor(a);
    unsigned short Y = (255.0 * a) - (255.0 * X);
    unsigned short RY = 255 - Y;
    switch ((int)X)
    {
    case 0:
        return {255, Y, 0};
    case 1:
        return {RY, 255, 0};
    case 2:
        return {0, 255, Y};
    case 3:
        return {0, RY, 255};
    case 4:
        return {Y, 0, 255};
    case 5:
    default:
        return {255, 0, 255};
    }
}

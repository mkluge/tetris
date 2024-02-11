#include <Basics.h>

bool operator==(const Pixel &lhs, const Pixel &rhs)
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}
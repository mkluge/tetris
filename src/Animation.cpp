#include "Animation.h"

Animation::Animation()
{

}

Animation::~Animation()
{

}
void Animation::rotate(int direction, bool keepInside)
{
    // rotation matrix in 2D is
    // R = ( cos(a) -sin(a))
    //     ( sin(a)  cos(a))
    // so 90 degrees is
    // R = ( 0 -1 ) -> -y+x
    //     ( 1  0 )
    // -90 dregees is: 
    // R = ( 0  1 ) -> y-x
    //     ( -1 0 )
    
}

#include <stdio.h>
#include "geom.h"

int main(int argc, char** argv) {
    cart_pos_2d_t cart_pos;
    cart_pos.x = 100;
    cart_pos.y = 200;

    polar_pos_2d_t polar_pos = 
        convert_to_2d_polar_pos(&cart_pos);
    printf("Polar position: Length: %f, Theta: %f (deg)\n",
        polar_pos.length, polar_pos.theta);
    return 0;        
}
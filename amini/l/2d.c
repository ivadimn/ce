#include <math.h>
#include "geom.h"

cart_pos_2d_t convert_to_2d_cart_pos(const polar_pos_2d_t* polar_pos) {
    cart_pos_2d_t result;
    result.x = polar_pos->length * cos_deg(polar_pos->theta);
    result.y = polar_pos->length * sin_deg(polar_pos->theta);
    return result;
}

polar_pos_2d_t convert_to_2d_polar_pos(const cart_pos_2d_t* cart_pos) {
    polar_pos_2d_t result;
    result.length = sqrt(cart_pos->x * cart_pos->x + 
        cart_pos->y * cart_pos->y);
    result.theta = to_degree(atan(cart_pos->y / cart_pos->x));
    return result;    
}
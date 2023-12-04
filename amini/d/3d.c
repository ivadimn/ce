#include <math.h>
#include "geom.h"

cart_pos_3d_t convert_to_3d_cart_pos(const polar_pos_3d_t* polar_pos) {
    cart_pos_3d_t result;
    result.x = polar_pos->length * sin_deg(polar_pos->theta) * 
        cos_deg(polar_pos->phi);
    result.y = polar_pos->length * sin_deg(polar_pos->theta) * 
        sin_deg(polar_pos->phi);
    result.z = polar_pos->length * cos_deg(polar_pos->theta);    
    return result;
}

polar_pos_3d_t convert_to_3d_polar_pos(const cart_pos_3d_t* cart_pos) {
    polar_pos_3d_t result;
    result.length = sqrt(cart_pos->x * cart_pos->x + 
        cart_pos->y * cart_pos->y + cart_pos->z * cart_pos->z);
    result.theta = to_degree(acos(cart_pos->z / result.length));
    result.phi = to_degree(atan(cart_pos->y / cart_pos->x));
    return result;    
}
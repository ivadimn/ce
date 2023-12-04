#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "geom.h"

polar_pos_2d_t (*func_ptr)(cart_pos_2d_t*);

int main(int argc, char** argv) {

    void* handle = dlopen("/opt/geom/libgeom.so", RTLD_LAZY);
    if (!handle)  {
        fprintf(stderr, "%s\n", dlerror());
        exit(1);
    }

    func_ptr = dlsym(handle, "convert_to_2d_polar_pos");
    if (!func_ptr)  {
        fprintf(stderr, "%s\n", dlerror());
        exit(1);
    }
    
    cart_pos_2d_t cart_pos;
    cart_pos.x = 100;
    cart_pos.y = 200;


    polar_pos_2d_t polar_pos = func_ptr(&cart_pos);
    printf("Polar position: Length: %f, Theta: %f (deg)\n",
        polar_pos.length, polar_pos.theta);
    return 0;        
}
#ifndef _VSTR_ARRAY_
#define _VSTR_ARRAY_

#include "vstr.h"

typedef struct {
    vstr_t* array;
    size_t size;
    size_t capacity;
} vstr_array_t;

vstr_array_t* vstr_array_create(size_t capacity);
void vstr_array_free(vstr_array_t* arr);


#endif
#ifndef _VSTR_H_
#define _VSTR_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

typedef struct {
    uint8_t *data;
    size_t size;
    size_t length;
} vstr_t;  

vstr_t* vstr_create(uint8_t size);
void vstr_free(vstr_t* str);
size_t vstr_size(vstr_t* str);
size_t vstr_len(vstr_t* str);
void vstr_print(vstr_t* str, FILE* f);
void vstr_assign(vstr_t *str, const char* value);
vstr_t* vstr_dup(const char* source);
vstr_t* vstr_concat(vstr_t* left, vstr_t* right);

#endif

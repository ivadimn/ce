#ifndef _DICT_H_
#define _DICT_H_

#include <unistd.h>

typedef struct cell_t {
    size_t count;
    char* word;
} cell_t;

typedef struct dict_t {
    cell_t **data;          //данные
    size_t count;           //
} dict_t;


void file_handle(char* filename);



#endif
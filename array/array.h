#ifndef _ARRAY_H_
#define _ARRAY_H_
#include <stddef.h>

#define DEFAULT_SIZE 32
typedef char byte;
typedef unsigned char ubyte;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef char* string;
typedef void* pointer;

#define REALLOC(ptr, osize, nsize, t)                 \
    do                                                \
    {                                                 \
        t *tmp = (t*)malloc(sizeof(t) * (osize));     \
        memcpy(tmp, ptr, (osize) * sizeof(t));                    \
        if(tmp == NULL) {                             \
            printf("Ошибка распределения памяти\n");  \
            exit(1);                                  \
        }                                             \
        free(ptr);                                    \
        ptr = (t*) malloc(sizeof(t) * (nsize));       \
        if(ptr == NULL) {                             \
            printf("Ошибка распределения памяти\n");  \
            exit(1);                                  \
        }                                             \
        memcpy(ptr, tmp,  (osize) * sizeof(t));                   \
        free(tmp);                                    \
    } while(0)                          

typedef enum {
    A_TYPE_VALUE,
    A_TYPE_STRING,
    A_TYPE_POINTER
} atype_t;

typedef struct array_t {
    atype_t type;
    int *valids;
    size_t size;
    size_t size_pointer;
    ubyte* arr;
} array_t;

array_t* new_array(atype_t type, size_t size, size_t size_pointer);
void del_array(array_t* arr);

void put(array_t* arr, size_t index, void* e);
void putm(array_t* arr, size_t index, void* p, size_t count);
void get(array_t* arr, size_t index, void* e);

#endif
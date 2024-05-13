#ifndef _ARRAY_H_
#define _ARRAY_H_

#include <stddef.h>
#include <string.h>

#define CMP_EQ(a, b)  (strcmp((char*)(a), (char*)(b)) == 0)

typedef struct {
    unsigned char* key;
    long value;
} data_t;


typedef struct {
    data_t *data;
    size_t size;
    size_t length;
} array_t;

array_t*  array_create(size_t size);
void array_delete(array_t* arr);

size_t array_put(array_t*arr, unsigned char* key, long value);
data_t* array_data(array_t* arr);
size_t array_length(array_t* arr);
void get_elements(array_t *arr, unsigned char** keys, long* values);



#endif
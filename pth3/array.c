#include "array.h"
#include "utils.h"
#include <stdlib.h>


static data_t* alloc_data(size_t size) {
    data_t *data = (data_t*) alloc(sizeof(data_t) * size);
    return data;
}

static void free_data(data_t *data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        free(data[i].key);
    }
    free(data);
}


array_t*  array_create(size_t size) {
    array_t *arr = NULL;
    arr = (array_t*) alloc(sizeof(array_t));
    arr->data = alloc_data(size);
    arr->length = 0;
    arr->size = size;
    return arr;
}

void array_delete(array_t* arr) {
    free_data(arr->data, arr->length);
    free(arr);
}

static void _resize(array_t* arr) {
    data_t *old_data = arr->data;
    data_t *new_data = alloc_data(arr->size * 2);
    for (size_t i = 0; i < arr->length; i++) {
        new_data[i].key = old_data[i].key;
        new_data[i].value = old_data[i].value;
    }
    arr->size *= 2;
    arr->data = new_data;  
    free(old_data);    
}

static long _index(array_t* arr, unsigned char* key) {
    for (size_t i = 0; i < arr->length; i++)
        if (CMP_EQ(arr->data[i].key, key)) 
            return i;
    return -1;
}

static long _put(array_t* arr, unsigned char* key, long value) {
    size_t len = arr->length;
    if ((len + 1) >= arr->size) 
        _resize(arr);

    arr->data[len].key = (unsigned char*) alloc(sizeof(unsigned char) * (strlen((char*)key) + 1));
    strcpy((char*)arr->data[len].key, (char*)key);
    arr->data[len].value = value;
    arr->length += 1;
    
    return len;
}

size_t array_put(array_t* arr, unsigned char* key, long value) {
    long idx = _index(arr, key);
    if (idx == -1) {
        idx = _put(arr, key, value);
    } else {
        arr->data[idx].value += value; 
    }
    return idx;  
}

data_t* array_data(array_t* arr) {
    return arr->data;
}

size_t array_length(array_t* arr) {
    return arr->length;
}

void get_elements(array_t *arr, unsigned char** keys, long* values) {
    for (size_t i = 0; i < arr->length; i++) {
        keys[i] = arr->data[i].key;
        values[i] = arr->data[i].value;
    }
}
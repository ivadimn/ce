#include "array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int _isvalid(array_t *arr, size_t index) {
    return 1 ? arr->valids[index] >= 0 : 0;
}

static void _valid(array_t *arr) {
    arr->valids = (int*) malloc(arr->size * sizeof(int));
    for (size_t i = 0; i < arr->size; i++) {
        arr->valids[i] = -1;
    }
}

static void _revalid(array_t *arr, size_t index) {
    for (size_t i = 0; i < arr->size; i++) {
        printf("%d, ", arr->valids[i]);
    }
    printf("\n");

    size_t osize = arr->size;
    size_t nsize = arr->size+index;
    REALLOC(arr->valids, osize, nsize, int);

    for (size_t i = 0; i < arr->size + index; i++) {
        printf("%d, ", arr->valids[i]);
    }
    printf("\n");

    for (size_t i = arr->size; i < arr->size + index; i++) {
        arr->valids[i] = -1;
    }
    
}

static void _alloc(array_t* arr) {
    _valid(arr);
    if(arr->valids == NULL) {
        printf("Ошибка распределения памяти!!!");
        exit(1);
    }
    arr->arr = (ubyte*) calloc(arr->size * arr->size_pointer,  sizeof(ubyte));
}

static void _realloc(array_t* arr, size_t index) {
    _revalid(arr, index);
    size_t osize = arr->size * arr->size_pointer;
    size_t nsize = (arr->size+index) * arr->size_pointer;
    REALLOC(arr->arr, osize, nsize, ubyte);
    arr->size += index;
  
    
}

static void _renew(array_t* arr, size_t index) {

    _realloc(arr, index); 
    if (arr->arr == NULL) {
        printf("Ошибка распределения памяти!!!");
        exit(1);
    }
}

array_t* new_array(atype_t type, size_t size, size_t size_pointer) {
    array_t *arr = malloc(sizeof(array_t));
    if (arr == NULL) {
        printf("Ошибка распределения памяти!!!");
        exit(1);
    }
    arr->valids = NULL;
    arr->size = size;
    arr->type = type;
    arr->size_pointer = size_pointer;
    _alloc(arr);
   
    if (arr->arr == NULL) {
        printf("Ошибка распределения памяти!!!");
        exit(1);
    }
    return arr;
}

void del_array(array_t* arr) {
    if (arr) {
        if(arr->arr)
            free(arr->arr);
        if (arr->valids) {
            free(arr->valids);
        }
        free(arr);    
    }
}

void put(array_t* arr, size_t index,  void* e) {

    ubyte* cp = NULL;
    if(index >= arr->size) {
        _renew(arr, index);
    }
    cp = arr->arr + (index * arr->size_pointer); 
    memcpy(cp, e, arr->size_pointer);
    arr->valids[index] = index;
    printf("from put %d\n", arr->valids[index]);
}

void putm(array_t* arr, size_t index, void* p, size_t count) {
    ubyte* cp = NULL;
    size_t last_index = index + count;
    if(last_index >= arr->size) {
        _renew(arr, last_index);
    }
    cp = arr->arr + (index * arr->size_pointer);
    memcpy(cp, p, (arr->size_pointer * count));
    for (size_t i = index; i < index + count; i++) {
        arr->valids[i] = i;
    }
}

void get(array_t* arr, size_t index,  void* e) {
    ubyte* cp = arr->arr;
    cp += index * arr->size_pointer;
    if (_isvalid(arr, index)) {
        printf("valid %d - ", arr->valids[index]);
        memcpy(e, cp, arr->size_pointer);
    }
    
}
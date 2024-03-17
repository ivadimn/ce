#include "array.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

int garr[] = {1023, 2047, 4095, 8911};

/*char* get_buffer(size_t size) {
    unsigned char* buffer = malloc(size);
    return buffer;
}

void put(unsigned char* buffer) {
    size_t shift = sizeof(int);
    unsigned char mask = 0xff;
    int e = 0; 
    for (size_t i = 0; i < 4; i++) {
        e = arr[i];
        for (size_t j = 0; j < shift; j++) {
            *buffer = (unsigned char)(e >> (j * 8)) & mask;
            buffer++;
        }
    }
}

void put1(unsigned char* buffer) {
    size_t shift = sizeof(int);
    unsigned char mask = 0xff;
    int e = 0; 
    for (size_t i = 0; i < 4; i++) {
        e = arr[i];
        memcpy(buffer, &e, shift);
        buffer += shift;
    }
}


void put2(unsigned char* buffer, void* a) {
    size_t shift = sizeof(int);
    unsigned char mask = 0xff;
    int e = 0; 
    for (size_t i = 0; i < 4; i++) {
        e = arr[i];
        memcpy(buffer, &e, shift);
        buffer += shift;
    }
    printf("%ld\n", sizeof(arr));

}

void get(unsigned char* buffer) {
    size_t shift = sizeof(int);
    unsigned char mask = 0xff;
    int e = 0, p = 0;
    for (size_t i = 0; i < 4; i++) {
        e = 0;
        for (size_t j = 0; j < shift; j++) {
            p = *buffer;
            e += (p << (j * 8));
            buffer++;
        }
        printf("%d\n", e);
    }

}

void get1(unsigned char* buffer) {
    size_t shift = sizeof(int);
    unsigned char mask = 0xff;
    int e = 0, p = 0;
    for (size_t i = 0; i < 4; i++) {
        e = 0;
        memcpy(&e, buffer, shift);
        buffer += shift;
        printf("%d\n", e);
    }

} 

void show(void* p ) {
    printf("array size from show - %lu\n", sizeof(garr));    
} */

int main()
{
    int e = 0;
    array_t *arr = new_array(A_TYPE_VALUE, 5, sizeof(int));
    
    for (size_t i = 0; i < 8; i++) {
        e = 10 + i;
        put(arr, i, &e);
    }
    putm(arr, 12, garr, 4);
    printf("\n");
    for (size_t i = 0; i < arr->size; i++) {
        get(arr, i, &e);
        printf("%d\n", e);
    }
    printf("array size - %lu", arr->size);
    return 0;
}

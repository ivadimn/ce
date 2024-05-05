#include "sort.h"
#include <stdio.h>


void sort(char** keys, long* values, size_t left, size_t right) {
    size_t i, j;
    for (i = left + 1; i <= right; i++)   {
        for (j = i; j > left; j--) {
            
            if(less(values[j - 1], values[j])) //{
                exch(keys[j], keys[j - 1], values[j], values[j - 1], long) 
                //printf("before values[j - 1]: %ld, values[j] %ld\n", values[j - 1], values[j]);
                //printf("before keys[j - 1]: %s, keys[j] %s\n", keys[j - 1], keys[j]);
                //char* tk = keys[j - 1]; keys[j - 1] = keys[j]; keys[j] = tk; 
                //long tv = values[j - 1]; values[j - 1] = values[j]; values[j] = tv; 
                //printf("after values[j - 1]: %ld, values[j] %ld\n", values[j - 1], values[j]);
                //printf("after keys[j - 1]: %s, keys[j] %s\n", keys[j - 1], keys[j]);
            //}
               
        }
    }
    
} 
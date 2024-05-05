#ifndef _SORT_H_
#define _SORT_H_

#include <stddef.h>

#define less(a, b) ((a) < (b))
#define exch(ka, kb, va,  vb, type) { \
    char* tk = ka; ka = kb; kb = tk; \
    type tv = va; va = vb; vb = tv; }
#define exch_val(a, b, type) { type tmp = a; a = b; b = tmp; }
#define compexch(a, b, type) if (less(b, a)) exch(a, b, type)

void sort(char** keys, long* values, size_t left, size_t right); 

#endif
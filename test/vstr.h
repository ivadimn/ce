#ifndef _VSTR_H_
#define _VSTR_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_PART 8192
typedef enum state { ONE, TWO } state_t;


typedef struct {
    uint8_t *data;
    long size;
    long length;
} vstr_t;  

typedef struct {
    vstr_t** array;
    long size;
    long length;
} vstr_array_t;



vstr_t* vstr_create(long size);
vstr_t* vstr_plus(long count, ...);
void vstr_free(vstr_t* str);
void vstr_clear(vstr_t* str);
size_t vstr_size(vstr_t* str);
size_t vstr_len(vstr_t* str);
void vstr_print(vstr_t* str, FILE* f);
void vstr_print_data(vstr_t* str, FILE* f);
void vstr_assign(vstr_t *str, const char* value);
vstr_t* vstr_dup(const char* source);
vstr_t* vstr_concat(vstr_t* left, vstr_t* right);
/* добавляет к строке набор байт возвращает новую строкуб старую строку удаляет*/
vstr_t* vstr_append(vstr_t* left, const char* right);
void vstr_split(vstr_array_t* arr, vstr_t* str, char* delim, vstr_t* g_open, vstr_t* g_close);
long vstr_in(vstr_t *str, char ch);
long vstr_instr(vstr_t *str, char* s);
vstr_t* vstr_substr(vstr_t *str, long start, long end);
uint8_t vstr_at(vstr_t *str, long index);
void vstr_put_ch(vstr_t *str, char ch);
void vstr_urldecode(vstr_t *str);
void vstr_cut(vstr_t* str, long count, int where);

vstr_array_t* vstr_array_create(long size);
void vstr_array_free(vstr_array_t* arr);
void vstr_array_clear(vstr_array_t* arr);
long vstr_array_addv(vstr_array_t* arr, vstr_t* str);
long vstr_array_adds(vstr_array_t* arr, const char* str);
vstr_t* vstr_array_get(vstr_array_t* arr, long index);
vstr_t* vstr_array_join(vstr_array_t* arr, char* delim);
void vstr_array_print(vstr_array_t* arr, FILE* f);
#endif

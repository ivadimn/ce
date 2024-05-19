#ifndef _UTILS_H_
#define _UTILS_H_

#include <stddef.h>

#define MAX_PATH 1024
#define MAX_PART 8192

//typedef enum state { ONE, TWO } state_t;


void* alloc(size_t size);

void split(char** arr, char* str, char delim, char* g_open, char* g_close);
long str_in(char *str, char ch);

long str_instr(char *str, char* s);
char* str_substr(char *str, long start, long end);
void str_urldecode(char *str);

#endif
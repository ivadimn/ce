#ifndef _FILE_INFO_H_
#define _FILE_INFO_H_

#include <stddef.h>

#define LINE_SIZE 16384
#define MAX_URL 4096

typedef enum {HOST, ID, USER, DATE, REQUEST, 
            STATUS, BYTES, REFERER, AGENT, END, COUNT} comb_t;

void handle_file(unsigned char* filename);
void handle_file1(unsigned char* filename);
#endif
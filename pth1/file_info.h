#ifndef _FILE_INFO_H_
#define _FILE_INFO_H_

#include <stddef.h>

#define LINE_SIZE 16384
#define ARR_SIZE 4096 * 1024

typedef enum {HOST, ID, USER, DATE, REQUEST, 
            STATUS, BYTES, REFERER, AGENT, END, COUNT} comb_t;


void init_arrs();
void remove_arrs();
void init_dicts();
void remove_dicts();
void handle_file(char* filename);
void handle_file1(char* filename);
size_t get_hosts(unsigned char*** hosts, long** bytes);
void remove_hosts(unsigned char*** hosts, long** bytes);


#endif
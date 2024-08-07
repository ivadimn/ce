#ifndef _UTILS_H_
#define _UTILS_H_

#include <stddef.h>

#define MAX_PATH 2048
#define MAX_PART 8192
#define BLOCK_SIZE 8192


char** init_file_list(size_t count);
void free_file_list(char** flist, size_t count);

size_t get_file_size(int fd);
int is_dir(const char* file);
size_t check_file(const char* filename);
int read_file(const char* filename, size_t fsize, char* buffer, long buf_off);

void file_list(char *dir, char** flist);
size_t get_count_files(char *dir);
void* alloc(size_t size);

#endif
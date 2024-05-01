#ifndef _FILE_INFO_H_
#define _FILE_INFO_H_

#include <stddef.h>

#define MAX_PATH 1024
#define LINE_SIZE 4096

size_t get_file_size(int fd);
int is_dir(const char* file);
void file_list(char *dir, char** flist);
size_t get_count_files(char *dir);
char** init_file_list(size_t count);
void handle_file(char* filename);


#endif
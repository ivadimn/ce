#ifndef _FILE_INFO_H_
#define _FILE_INFO_H_

#include <stddef.h>

#define MAX_PATH 1024

typedef enum {TYPE_NONE, TYPE_DIR, TYPE_FILE} file_t;

typedef struct file_info_t {
    char name[MAX_PATH];
    size_t size;
    file_t type;
} file_info_t;

void get_file_info(const char* file, file_info_t* finfo);
void dir_list( char *dir, int tc);


#endif
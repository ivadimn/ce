#ifndef _FILE_INFO_H_
#define _FILE_INFO_H_

#include <stddef.h>
#include <time.h>

#define MAX_PATH 1024
#define MAX_NAME 256

typedef enum {TYPE_NONE, TYPE_DIR, TYPE_FILE} file_t;

typedef struct file_info_t {
    char full_name[MAX_PATH];
    char name[MAX_NAME];
    size_t size;
    file_t type;
    struct tm date;
    struct file_info_t *flist;
} file_info_t;

void file_info(file_info_t* finfo);
void get_file_list(file_info_t* dir);
file_info_t* create_dir( char *dir);


#endif
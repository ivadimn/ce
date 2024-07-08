#include "utils.h"
#include <sys/stat.h>


int is_valid_file(char* filename, char* err_msg) {

    struct stat stat_buff;
    int result = stat(filename, &stat_buff);
    if (result == -1) {
        err_cont("Ошибка при получении информации о файле: %s", finfo->full_name);
        finfo->type = TYPE_NONE;
        return;
    }
    
    if (S_ISDIR(stat_buff.st_mode)) {
        finfo->type = TYPE_DIR;
        finfo->size = get_dir_size(finfo->full_name);
    }
    else {
        finfo->type = TYPE_FILE;
        finfo->size = stat_buff.st_size;
    }
    finfo->flist = NULL;
    finfo->date = stat_buff.st_ctime;

}





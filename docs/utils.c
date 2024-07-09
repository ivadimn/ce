#include "def.h"
#include "utils.h"
#include <stdio.h>
#include <sys/stat.h>


int is_valid_file(char* filename, char* err_msg) {

    struct stat stat_buff;
    int result = stat(filename, &stat_buff);
    if (result == -1) {
        snprintf(err_msg, ERR_MSG_LEN - 1, 
                "Ошибка при получении информации о файле: %s", filename);
        return -1;
    }
    
    if (S_ISDIR(stat_buff.st_mode)) {
        snprintf(err_msg, ERR_MSG_LEN - 1, 
                "Указанный файл: %s является каталогом" , filename);
        return -1;
    }
    return 0;
}





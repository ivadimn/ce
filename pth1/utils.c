#include "utils.h"
#include "log.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>

/*
* подготавливает массив размером count для списка файлов
*/
char** init_file_list(size_t count) {
    char** flist = NULL;
    flist = (char**) alloc(count * sizeof(char*));
    
    for (size_t i = 0; i < count; i++) {
        flist[i] = (char*)alloc(sizeof(char) * MAX_PATH);
    }
    return flist;
}

/*
* удаляет массив для списка файлов
*/
void free_file_list(char** flist, size_t count) {
    for (size_t i = 0; i < count; i++) {
        free(flist[i]);
    }
    free(flist);
}

/*
* получить размер файла
*/
size_t get_file_size(int fd) {
    
	int64_t fsize = 0;
	struct stat fileStatbuff;
	if ((fstat(fd, & fileStatbuff) != 0) || (!S_ISREG(fileStatbuff.st_mode))) {
		fsize = 0;
	}
	else{
		fsize = fileStatbuff.st_size;
        
	}
	return fsize;
}


/*
* проверяет является ли файл - каталогом
*/
int is_dir(const char* file) {
    struct stat stat_buff;
    int result = stat(file, &stat_buff);
    if (result == -1) {
        err("Error while getting file info %s", file);
        return 0;
    }
    if (S_ISDIR(stat_buff.st_mode)) {
        return 1;
    }
    return 0;
}

/*
* получить количество файлов в каталоге
*/
size_t get_count_files(char *dir) {

    char name[MAX_PATH];
    struct dirent *dp = NULL;
    DIR *dfd = NULL;
    size_t count = 0;

    if((dfd = opendir(dir))==NULL){
        err("Ошибка открытия каталога (in get_dir_size): %s", dir);
        return 0;
    }

    while((dp=readdir(dfd)) != NULL){
        
        if(strcmp(dp->d_name,".") == 0 || 
            strcmp(dp->d_name,"..") == 0)
            continue;

        sprintf(name,"%s/%s",dir,dp->d_name);
        if (is_dir(name))
            continue;

        count++;    
    }
    closedir(dfd); 
    return count;   
}

/*
* получаем список файлов в каталоге
*/
void file_list(char *dir, char** flist) {

    char name[MAX_PATH];
    struct dirent *dp;
    DIR *dfd;
    int index = 0;
    if((dfd = opendir(dir))==NULL){
        err("Невозможно открыть каталог: %s", dir);
        return;
    }

    while((dp=readdir(dfd)) != NULL){
        
        if(strcmp(dp->d_name,".") == 0
            || strcmp(dp->d_name,"..") ==0 )
            continue;
        
        sprintf(name,"%s/%s",dir,dp->d_name);
        if (is_dir(name))
            continue;
        
        strcpy(flist[index++], name);
    }
    closedir(dfd);
}

/*
* разбивает строку на подстроки по разделителю delim и вывделяет группы
* группа символов начинается g_open символом и заканчиваетяс g_close символом
* номера индексов сиволов открытия и закрытия группы должны совпадать в g_open
* и g_close
*/
void split(char** arr, char* str, char delim, char* g_open, char* g_close) {
    long len = strlen(str); 
    long part_index = 0, arr_index = 0;
    int  in_group = 0;
    long g_index = -1;

    char part[MAX_PART];
    
    for (long i = 0; i < len; i++) {
        
        switch (in_group)  {
            case 0:
                if (str[i] == delim) {
                    part[part_index] = '\0';
                    strcpy(arr[arr_index++], part);
                    part_index = 0;
                    
                } 
                else if(g_open && (g_index = str_in(g_open, str[i])) >= 0) {
                    in_group = 1;
                }
                else {
                    part[part_index++] = str[i];
                }
                break;
            case 1:
                if (str[i] == g_close[g_index]) {
                    in_group = 0;
                    g_index = -1;
                }
                else {
                    part[part_index++] = str[i];
                }
                break;
            default:
                break;
        }
    }
    part[part_index] = '\0';
    strcpy(arr[arr_index], part);
}

/*
* возвращает первое вхождение символа в строку
*/
long str_in(char *str, char ch) {
    long len = (long)strlen(str);
    for (long i = 0; i < len; i++)  {
        if (str[i] == ch)
            return i;
    }
    return -1;
}

void* alloc(size_t size) {
    void *p = malloc(size);
    
    if (p == NULL)
        crit("Ошибка распределения памяти: ");
    return p;
}



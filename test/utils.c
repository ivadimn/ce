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

void* alloc(size_t size) {
    void *p = malloc(size);
    
    if (p == NULL)
        crit("Ошибка распределения памяти: ");
    return p;
}



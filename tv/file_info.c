#include "common.h"
#include "file_info.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>


void get_file_info(file_info_t* finfo) {
    struct stat stat_buff;
    int result = stat(finfo->name, &stat_buff);
    if (result == -1) {
        err_cont("Error while getting file info %s", finfo->name);
        finfo->type = TYPE_NONE;
        return;
    }
    
    if (S_ISDIR(stat_buff.st_mode)) {
        finfo->type = TYPE_DIR;
        finfo->size = 0;
    }
    else {
        finfo->type = TYPE_FILE;
        finfo->size = stat_buff.st_size;
    }
    return;
}

void get_file_list(file_info_t *dir) {

    char name[MAX_PATH];
    file_info_t finfo;
    struct dirent *dp;
    DIR *dfd;
    size_t index = 0;

    if((dfd = opendir(dir))==NULL){
        err_cont("Error open dir: %s", dir);
        return NULL;
    }

    dir->flist = (file_info_t*) malloc(sizeof(file_info_t) * dir->size);
    if (dir->flist == NULL) {
        err_sys("Ошибка выделения памяти!");
    }
    
    while((dp=readdir(dfd)) != NULL){
        
        if(strcmp(dp->d_name,".") == 0
            || strcmp(dp->d_name,"..") ==0 )
            continue;
        sprintf(name,"%s/%s",dir,dp->d_name);
        strcpy(dir->flist[index].name, dp->d_name);
        strcpy(dir->flist[index].full_name, name);        
        get_file_info(&dir->flist[index]);
        index++;
    }
    closedir(dfd);
}

file_info_t* create_dir( char *dir) {

    char name[MAX_PATH];
    file_info_t *finfo = NULL;
    struct dirent *dp = NULL;
    DIR *dfd = NULL;
    size_t count = 0;

    if((dfd = opendir(dir))==NULL){
        err_cont("Ошибка открытия каталога: %s", dir);
        return NULL;
    }

    finfo = (file_info_t*) malloc(sizeof(file_info_t));
    if (finfo == NULL) {
        err_sys("Ошибка выделения памяти!");
    }
    finfo->type = TYPE_DIR;
    strcpy(finfo->name, dir);
    strcpy(finfo->full_name, dir);
    finfo->flist = NULL;
    
    while((dp=readdir(dfd)) != NULL){
        
        if(strcmp(dp->d_name,".") == 0
            || strcmp(dp->d_name,"..") ==0 )
            continue;
        count++;    
    }
    finfo->size = count;
    closedir(dfd);
    return finfo;
}
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

file_info_t* create_file_list( char *dir) {

    char name[MAX_PATH];
    file_info_t finfo;
    struct dirent *dp;
    DIR *dfd;

    if((dfd = opendir(dir))==NULL){
        err_cont("Error open dir: %s", dir);
        return NULL;
    }
    
    while((dp=readdir(dfd)) != NULL){
        
        if(strcmp(dp->d_name,".") == 0
            || strcmp(dp->d_name,"..") ==0 )
            continue;
        
        sprintf(name,"%s/%s",dir,dp->d_name);
        get_file_info(name, &finfo);
        if(finfo.type == TYPE_DIR) {
            printf("%s\n", dp->d_name);
            dir_list(name);
        }
        else {    
            printf("%s %ld %d\n", dp->d_name, finfo.size, finfo.type);
        }
    }
    closedir(dfd);
}
#include "common.h"
#include "file_info.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <pwd.h>



void get_file_info(const char* file, file_info_t* finfo) {
    struct stat stat_buff;
    int result = stat(file, &stat_buff);
    if (result == -1) {
        err_cont("Error while getting file info %s", file);
        finfo->type = TYPE_NONE;
        return;
    }
    strcpy(finfo->name, file);
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

void dir_list( char *dir, int tc) {

    char name[MAX_PATH];
    char tabs[16] = {0}; 
    int tabc = tc;
    file_info_t finfo;
    struct dirent *dp;
    DIR *dfd;

    if((dfd = opendir(dir))==NULL){
        puts("Error: Cannot open Directory");
        return;
    }
    
    size_t i = 0;
    for (; i < tabc; i++) {
        tabs[i] = '\t';
    }
    tabs[i] = '\0';
    
    
    while((dp=readdir(dfd)) != NULL){
        
        if(strcmp(dp->d_name,".") == 0
            || strcmp(dp->d_name,"..") ==0 )
            continue;
        
        sprintf(name,"%s/%s",dir,dp->d_name);
        get_file_info(name, &finfo);
        if(finfo.type == TYPE_DIR) {
            printf("%s%s\n", tabs, dp->d_name);
            dir_list(name, tc + 1);
        }
        else {    
            printf("%s%s %ld %d\n", tabs, dp->d_name, finfo.size, finfo.type);
        }
    }
    closedir(dfd);
}
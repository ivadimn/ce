/* these defines at beginning to highlight them */
//#define S_IFMT 0160000 /* type of file: */
//#define S_IFDIR 0040000 /* directory */

/*
   Modify the fsize program to print the other information contained in the inode entry.
   */
#include "common.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>


#define MAX_PATH 1024

#ifndef DIRSIZ
#define DIRSIZ 14
#endif

typedef enum {TYPE_NONE, TYPE_DIR, TYPE_FILE} file_t;

typedef struct file_info_t {
    char name[MAX_PATH];
    size_t size;
    file_t type;
} file_info_t;

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


void dirwalk( char *dir, int tc){

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
    
    //puts(dir);
    printf("%s%s\n", tabs, finfo.name);
    // Get each dir entry
    while((dp=readdir(dfd)) != NULL){
        // Skip . and .. is redundant.
        
        if(strcmp(dp->d_name,".") == 0
            || strcmp(dp->d_name,"..") ==0 )
            continue;
        /*if(strlen(dir)+strlen(dp->d_name)+2 > sizeof(name))
            puts("Error: Name too long!");
        else{
            sprintf(name,"%s/%s",dir,dp->d_name);
            // Call fsize
            (*fcn)(name);
        }*/
        sprintf(name,"%s/%s",dir,dp->d_name);
        //printf("%s/   %s\n",dir,dp->d_name);
        get_file_info(name, &finfo);
        if(finfo.type == TYPE_DIR)
            dirwalk(name, tc + 1);
        else {    
            printf("%s%s %ld %d\n", tabs, dp->d_name, finfo.size, finfo.type);
        }
    }
    closedir(dfd);
}


void fsize(char *name){
    struct stat stbuf;

    if(stat(name,&stbuf) == -1){
        puts("Error: Cannot get file stats!");
        return;
    }

    if((stbuf.st_mode & S_IFMT) == S_IFDIR){
        dirwalk(name,0);
    }
    struct passwd *pwd = getpwuid(stbuf.st_uid);
    //print file name,size and owner
    printf("%1d %s Owner: %s\n",(int)stbuf.st_size,name,pwd->pw_name);
}

int main(int argc,char *argv[]){

    if(argc==1)
        dirwalk("/home/vadim/ce", 0);
    else 
        while(--argc>0)
            dirwalk(*++argv, 0);
    return 0;
}
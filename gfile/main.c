/* these defines at beginning to highlight them */
//#define S_IFMT 0160000 /* type of file: */
//#define S_IFDIR 0040000 /* directory */

/*
   Modify the fsize program to print the other information contained in the inode entry.
   */

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>
#include <pwd.h>


#define MAX_PATH 1024

#ifndef DIRSIZ
#define DIRSIZ 14
#endif

typedef enum {TYPE_DIR, TYPE_FILE} file_t;

typedef struct file_info_t {
    char name[MAX_PATH];
    size_t size;
    file_t type;
} file_info_t;

void get_file_info(const char* file, file_info_t* finfo) {

}


void dirwalk( char *dir,void (*fcn)(char *)){

    char name[MAX_PATH];
    struct dirent *dp;
    DIR *dfd;

    if((dfd = opendir(dir))==NULL){
        puts("Error: Cannot open Directory");
        return;
    }
    
    puts(dir);
    // Get each dir entry
    while((dp=readdir(dfd)) != NULL){
        // Skip . and .. is redundant.
        
        /*if(strcmp(dp->d_name,".") == 0
            || strcmp(dp->d_name,"..") ==0 )
            continue;
        if(strlen(dir)+strlen(dp->d_name)+2 > sizeof(name))
            puts("Error: Name too long!");
        else{
            sprintf(name,"%s/%s",dir,dp->d_name);
            // Call fsize
            (*fcn)(name);
        }*/
        printf("%s/   %s\n",dir,dp->d_name);
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
        dirwalk(name,fsize);
    }
    struct passwd *pwd = getpwuid(stbuf.st_uid);
    //print file name,size and owner
    printf("%1d %s Owner: %s\n",(int)stbuf.st_size,name,pwd->pw_name);
}

int main(int argc,char *argv[]){

    if(argc==1)
        fsize("/home/vadim");
    else 
        while(--argc>0)
            fsize(*++argv);
    return 0;
}
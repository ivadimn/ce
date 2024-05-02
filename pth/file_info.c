#include "log.h"
#include "file_info.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/mman.h>

static void get_lines(char* buf);
static void get_lines1(FILE* f);

char** init_file_list(size_t count) {
    char** flist = NULL;
    flist = (char**) malloc(count * sizeof(char*));
    if (flist == NULL)
        crit("Ошибка распределения памяти");
    
    for (size_t i = 0; i < count; i++) {
        flist[i] = (char*)malloc(sizeof(char) * MAX_PATH);
        if (flist[i] == NULL)
            crit("Ошибка распределения памяти");
    }
    return flist;
}

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

void free_file_list(char** flist, size_t count) {
    for (size_t i = 0; i < count; i++) {
        free(flist[i]);
    }
    free(flist);
}

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

void file_list(char *dir, char** flist) {

    char name[MAX_PATH];
    struct dirent *dp;
    DIR *dfd;
    int index = 0;
    if((dfd = opendir(dir))==NULL){
        err("Error: Cannot open Directory");
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

void handle_file(char* filename) {

    int fd;
    size_t fsize;
    char *fbuff = NULL;
    

    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        err("Ошибка открытия файла: %s", filename);
        return;
    }
    printf("файл открыт\n");

    fsize = get_file_size(fd);
    if (fsize == 0) {
        crit("Ошибка при определении размера файла: %s", filename);
    }
    printf("размер файла %ld\n", fsize);

    fbuff = (char*) mmap(NULL, fsize, PROT_READ, MAP_PRIVATE, fd, 0);

    if (fbuff == NULL) {
        crit("Ошибка распределения памяти");
    }
    
    printf("файл прочитан\n");
    get_lines(fbuff);

    munmap(fbuff, fsize);

    close(fd);
}

void handle_file1(char* filename) {

    FILE* fd;
    size_t fsize;
    char *fbuff = NULL;
    

    fd = fopen(filename, "r");
    if (fd == NULL) {
        err("Ошибка открытия файла: %s", filename);
        return;
    }
    
    get_lines1(fbuff);

    fclose(fd);
}

static void get_lines(char* buf) {

    char line[LINE_SIZE];

    size_t index = 0, count = 0;

    while (*buf != '\0') {
        if (*buf == '\n')  {
            line[index] = '\0';
            index = 0;
            printf("%s\n", line);
        }
        else {
            line[index++] = *buf;
        }
        //count++;
        buf++;
        //printf("count = %ld", count);
    }
    printf("finish\n");

}


static void get_lines1(FILE* f) {

    char ip[16];

    size_t index = 0, count = 0;

    while (fscanf(f, "")) {
        if (*buf == '\n')  {
            line[index] = '\0';
            index = 0;
            printf("%s\n", line);
        }
        else {
            line[index++] = *buf;
        }
        //count++;
        buf++;
        //printf("count = %ld", count);
    }
    printf("finish\n");

}
#include "common.h"
#include "file_info.h"
#include "vstr.h"
#include "utils.h"
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>


#define URL_PC  3

const size_t buff_size = 8 * 1024 * 1024;

static void save_data(unsigned char** arr) {

    unsigned char* url;
    unsigned char* host = arr[HOST];
    unsigned char* request = arr[REQUEST];
    unsigned char* ref = arr[REFERER];

    long start = str_in((char*)request, '/');
	long end = str_instr((char*)request, " HTTP");
	
	url = (unsigned char*) str_substr((char*)request, start, end);
    //printf("%s\n", url);
	str_urldecode((char*)url);
    printf("%s, %lu\n", host, strlen((char*) host));
    printf("%s\n", url);
	//strcat((char*)host, (char*)url);
    //str_urldecode((char*) ref);
	FILE *f = fopen("test.txt", "a+b");
    fwrite(host, sizeof(char), strlen((char*)host), f);
    fwrite("\n", sizeof(char), 1, f);
    fwrite(url, sizeof(char), strlen((char*)url), f);
    fwrite("\n", sizeof(char), 1, f);
    //fwrite(ref, sizeof(char*), strlen((char*)ref), f);
    //fwrite("\n\n", sizeof(char*), 2, f);*/
    printf("-------------------------------------------------------\n");
    fclose(f);
    free(url);
}

static void save_data1(vstr_array_t *arr) {

    vstr_t* url;
    vstr_t* host = vstr_array_get(arr, HOST);
    vstr_t* request = vstr_array_get(arr, REQUEST);
    vstr_t* ref = vstr_array_get(arr, REFERER);

    long start = vstr_in(request, '/');
	long end = vstr_instr(request, " HTTP");
	
	url = vstr_substr(request, start, end);
	vstr_urldecode(url);
	vstr_t* full_url = vstr_concat(host, url);
    vstr_urldecode(ref);
	FILE *f = fopen("test.txt", "a+b");
    vstr_print(full_url, f);
    vstr_print(ref, f);
	printf("-------------------------------------------------------\n");
    fclose(f);
}

void handle_file1(unsigned char* filename) {
    int fd;
    size_t read_count = 0;
    long index = 0;
    vstr_t *g_open = vstr_dup("\"["); 
    vstr_t *g_close = vstr_dup("\"]"); 
    unsigned char* fbuff = (unsigned char*) malloc(sizeof(unsigned char) * buff_size);
    vstr_t* str = vstr_create(LINE_SIZE);
    vstr_array_t* arr = vstr_array_create(11);
        
    fd = open((char*)filename, O_RDONLY);
    
    if (fd == -1) {
        err_sys("Can not open file: %s", filename);
    }

    do {

        read_count = read(fd, fbuff, buff_size);
        for (size_t i = 0; i < read_count; i++) {
            if (fbuff[i] == '\n')  {
                index = 0;
                vstr_split(arr, str, ' ', g_open, g_close);
                save_data1(arr);
                vstr_array_clear(arr);
                vstr_clear(str);
            }
            else {
                vstr_put_ch(str, fbuff[i]);
                //line[index++] = fbuff[i];
            }
        }
    } while (read_count == buff_size);

    vstr_free(g_open);
    vstr_free(g_close);    
    vstr_free(str);
    vstr_array_free(arr);
    free(fbuff);    
    close(fd);
    printf("File: %s was handled\n", filename);
    
}

void handle_file(unsigned char* filename) {
    int fd;
    size_t read_count = 0;
    long index = 0;
    char *g_open = "\"["; 
    char *g_close = "\"]"; 
    unsigned char* line;
    unsigned char *fbuff = NULL;
    unsigned char** arr = NULL;
    
    fd = open((char*) filename, O_RDONLY);
    if (fd == -1) {
        err_sys("Can not open file: %s", filename);
      
    }

    line = (unsigned char*)alloc(sizeof(unsigned char) * LINE_SIZE);
    fbuff = (unsigned char*)alloc(sizeof(unsigned char) * buff_size);
    arr = (unsigned char**)alloc(sizeof(unsigned char*) * COUNT);
    for (size_t i = 0; i < COUNT; i++) {
        arr[i] = (unsigned char*)alloc(sizeof(unsigned char) * MAX_PART);
    }

    do {

        read_count = read(fd, fbuff, buff_size);
        for (size_t i = 0; i < read_count; i++) {
            if (fbuff[i] == '\n')  {
                line[index] = '\0';
                index = 0;
                //printf("%s\n", line);
                split((char**)arr, (char*)line, ' ', g_open, g_close);
                //printf("%s\n", arr[HOST]);
                //printf("%s\n", arr[REQUEST]);
                //printf("%s\n", arr[REFERER]);
                save_data((unsigned char**)arr);
            }
            else {
                line[index++] = fbuff[i];
            }
        }
    } while (read_count == buff_size);

    for (size_t i = 0; i < COUNT; i++) {
        free(arr[i]);
    }
    free(arr);
    free(fbuff);    
    free(line);
    close(fd);
    printf("Файл: %s обработан\n", filename);
    
}

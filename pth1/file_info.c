#include "log.h"
#include "file_info.h"
#include "utils.h"
#include "array.h"
#include "dict.h"
#include "sort.h"

#include <sys/mman.h>

const size_t buff_size = 8 * 1024 * 1024;

static dict_t *host_dict = NULL;
static dict_t *refer_dict = NULL;

static array_t *host_arr = NULL;
static array_t *refer_arr = NULL;

static void get_lines(unsigned char* buf);
static void save_data(unsigned char** arr); 

void init_dicts() {
    host_dict = create_dict(ARR_SIZE, 0.72f, 2.0f);
    refer_dict = create_dict(ARR_SIZE, 0.72f, 2.0f);
}

void remove_dicts() {
    destroy_dict(host_dict);
    destroy_dict(refer_dict); 
}

void init_arrs() {
    host_arr = array_create(ARR_SIZE);
    refer_arr = array_create(ARR_SIZE);
}

void remove_arrs() {
    array_delete(host_arr);
    array_delete(refer_arr);
}

void handle_file(char* filename) {

    int fd;
    size_t fsize;
    unsigned char *fbuff = NULL;
    

    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        err("Ошибка открытия файла: %s", filename);
        return;
    }

    fsize = get_file_size(fd);
    if (fsize == 0) {
        crit("Ошибка при определении размера файла: %s", filename);
    }
    fbuff = (unsigned char*) mmap(NULL, fsize, PROT_READ, MAP_PRIVATE, fd, 0);

    if (fbuff == NULL) {
        crit("Ошибка распределения памяти");
    }
        
    printf("Call getlines\n");    
    get_lines(fbuff);
    
    munmap(fbuff, fsize);
    close(fd);
    
}

void handle_file1(char* filename) {

    int fd;
    size_t read_count = 0;
    long index = 0;
    char *g_open = "\"["; 
    char *g_close = "\"]"; 
    unsigned char* line;
    unsigned char *fbuff = NULL;
    unsigned char** arr = NULL;
    
    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        err("Ошибка открытия файла: %s", filename);
        return;
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
                split((char**)arr, (char*)line, ' ', g_open, g_close);
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
}


static void get_lines(unsigned char* buf) {

    char *g_open = "\"["; 
    char *g_close = "\"]"; 
    unsigned char line[LINE_SIZE];    
    //unsigned char arr[COUNT][LINE_SIZE];
    unsigned char** arr = (unsigned char**)alloc(sizeof(unsigned char*) * COUNT);
    for (size_t i = 0; i < COUNT; i++) {
        arr[i] = (unsigned char*)alloc(sizeof(unsigned char) * MAX_PART);
    }
    
    long index = 0;

    while (*buf != '\0') {
        if (*buf == '\n')  {
            line[index] = '\0';
            index = 0;
            split((char**)arr, (char*)line, ' ', g_open, g_close);
            save_data((unsigned char**)arr);
        }
        else {
            line[index++] = *buf;
        }
        buf++;
    }
    for (size_t i = 0; i < COUNT; i++) {
        free(arr[i]);
    }
    free(arr);
    return;
}

static void save_data(unsigned char** arr)  {
    //long count = 1, value;
    //long rc = 0;
    long bytes = strtol((char*)arr[BYTES], NULL, 10);
    
    array_put(host_arr, arr[HOST], bytes);
    array_put(refer_arr, arr[REFERER], 1);
    
    /*rc  = get(host_dict, arr[HOST], &value);
    
    if (!rc) {
        put(&host_dict, arr[HOST], bytes);
    }
    else {
        value += bytes;
        put(&host_dict, arr[HOST], value);
    }
    rc = get(refer_dict, arr[REFERER], &value);
    if (!rc) {
        put(&refer_dict, arr[REFERER], count);
    }
    else {
        value += 1;
        put(&refer_dict, arr[REFERER], value);
    }*/
}

size_t get_hosts(unsigned char*** hosts, long** bytes) {
    //*hosts = (unsigned char**) alloc(sizeof(unsigned char*) * host_dict->count);
    //*bytes = (long*) alloc(sizeof(long) * host_dict->count);
    *hosts = (unsigned char**) alloc(sizeof(unsigned char*) * host_arr->length);
    *bytes = (long*) alloc(sizeof(long) * host_arr->length);
    get_elements(host_arr, *hosts, *bytes);
    //get_dict_elements(host_dict, *hosts, *bytes);
    sort_shell(*hosts, *bytes, 0, host_arr->length - 1);
    //sort_shell(*hosts, *bytes, 0, host_dict->count - 1);
    //return host_dict->count;
    return host_arr->length;
}

void remove_hosts(unsigned char*** hosts, long** bytes) {
    free(*bytes);
    free(*hosts);
}
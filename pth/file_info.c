#include "log.h"
#include "file_info.h"
#include "vstr.h"
#include "utils.h"
#include "dict.h"
#include "sort.h"

#include <sys/mman.h>

static dict_t *host_dict = NULL;
static dict_t *refer_dict = NULL;

static void get_lines(char* buf);
static void save_data(vstr_array_t* arr); 

void init_dicts() {
    host_dict = create_dict(4096, 0.72f, 2.0f, LONG);
    refer_dict = create_dict(4096, 0.72f, 2.0f, LONG);
}

void remove_dicts() {
    destroy_dict(host_dict);
    destroy_dict(refer_dict); 
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
    printf("get_lines отработала\n");

    munmap(fbuff, fsize);
    close(fd);
    print_dict_st(host_dict);
    print_dict_st(refer_dict);
}

static void save_data(vstr_array_t* arr)  {

    long *value, nc = 0, count = 1;
    vstr_t* elem = vstr_array_get(arr, BYTES);
    long bytes = strtol((char*)elem->data, NULL, 10);
    
    elem = vstr_array_get(arr, HOST);
    value = (long*) get(host_dict, elem->data);

    if (value == NULL) {
        put(&host_dict, elem->data, &bytes);
    }
    else {
        nc = *value += bytes;
        put(&host_dict, elem->data, &nc);
    }
    
    elem = vstr_array_get(arr, REFERER);
    value = (long*) get(refer_dict, elem->data);
    if (value == NULL) {
        put(&refer_dict, elem->data, &count);
    }
    else {
        nc = *value += 1;
        put(&refer_dict, elem->data, &nc);
    }
}


static void get_lines(char* buf) {

    vstr_t *str = vstr_create(LINE_SIZE);
    vstr_t *g_open = vstr_dup("\"["); 
    vstr_t *g_close = vstr_dup("\"]"); 
    vstr_array_t* arr = vstr_array_create(COUNT);
    size_t index = 0;

    char* line = (char*) alloc(sizeof(char*) * LINE_SIZE);

    while (*buf != '\0') {
        if (*buf == '\n')  {
            line[index] = '\0';
            index = 0;
            vstr_assign(str, line);
            vstr_split(arr, str, ' ', g_open, g_close);
            save_data(arr);
            vstr_array_clear(arr);
        }
        else {
            line[index++] = *buf;
        }
        buf++;
    }
    free(line);
    vstr_free(str);
    vstr_array_free(arr);
    vstr_free(g_open);
    vstr_free(g_close);
    printf("get_lines отработала in\n");
    return;
}

void get_hosts(char*** hosts, long** bytes) {
    *hosts = (char**) alloc(sizeof(char*) * host_dict->count);
    *bytes = (long*) alloc(sizeof(long) * host_dict->count);
    get_elements(host_dict, *hosts, *bytes);
    sort(*hosts, *bytes, 0, host_dict->count - 1);
}

void remove_hosts(char*** hosts, long** bytes) {
    char** tmp = *hosts;
    free(*bytes);
    for (size_t i = 0; i < 10; i++)  {
        free(tmp[i]);
    }
    free(*hosts);
}
#include "log.h"
#include "file_info.h"
#include "utils.h"
#include "tree.h"
#include "sort.h"
#include <pthread.h>

#include <sys/mman.h>
#define URL_PC  3

const size_t buff_size = 8 * 1024 * 1024;
static node_t* tree_hosts = NULL;
static node_t* tree_refers = NULL;

static void save_data(unsigned char** arr); 

static pthread_mutex_t mtx;

void remove_trees() {
    tree_delete(tree_hosts);
    tree_delete(tree_refers);
}
void init_mtx() {
    pthread_mutex_init(&mtx, NULL);
}
void release_mtx() {
    pthread_mutex_destroy(&mtx);
}

void *handle_file(void* arg) {
    char* filename = (char*)arg;
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
        pthread_exit(NULL);
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
    printf("Файл: %s обработан\n", filename);
    pthread_exit(NULL);
}

static void save_data(unsigned char** arr)  {
    
    long bytes = strtol((char*)arr[BYTES], NULL, 10);
    long url_start = -1, url_end = -1;
    
    unsigned char *full_url;

    pthread_mutex_lock(&mtx);
    if (bytes > 0) {
        url_start = str_in((char*)arr[REQUEST], '\\');
        url_end = str_in((char*)arr[REQUEST], 'H');
        if (url_start > 0 && url_end > 0) {
            full_url = (unsigned char*)strncat((char*)arr[HOST], (char*) &arr[REQUEST][url_start], url_end - url_start);
            push(&tree_hosts, full_url, bytes);
        }
        else {
            push(&tree_hosts, arr[HOST], bytes);
        }
    }
    push(&tree_refers, arr[REFERER], 1);
    pthread_mutex_unlock(&mtx);
    
}

size_t get_hosts(unsigned char*** hosts, size_t** bytes) {
    size_t hosts_count = tree_count_nodes(tree_hosts);
    *hosts = (unsigned char**) alloc(sizeof(unsigned char*) * hosts_count);
    *bytes = (size_t*) alloc(sizeof(size_t) * hosts_count);
    get_leafs(tree_hosts, *hosts, *bytes);
    sort_shell(*hosts, *bytes, 0, hosts_count - 1);
    return hosts_count;
}

void remove_hosts(unsigned char*** hosts, size_t** bytes) {
    free(*bytes);
    free(*hosts);
}
#include "common.h"
#include "dict.h"
#include <fcntl.h>
#include <stdint.h>
#include <ctype.h>
#include <locale.h>

static const float DEFAULT_FACTOR = 0.72f;
static const size_t DEFAULT_SIZE = 256;
static const float MULT = 2.0f;

dict_t* create_dict(size_t size, float factor, float mult) {
    dict_t *dict = (dict_t*) malloc(sizeof(dict_t));
    dict->count = 0;
    dict->size = (size >= DEFAULT_SIZE) ? size : DEFAULT_SIZE;
    dict->factor = (factor >= DEFAULT_FACTOR && factor < 1.0f ) 
                    ? factor : DEFAULT_FACTOR;
    dict->limit = (size_t) (dict->factor * dict->size);
    dict->mult = (mult >= MULT) ? mult : MULT;
    dict->data = (cell_t**) calloc(dict->size, sizeof(cell_t));                 
    return dict;
}

void put(dict_t **dict, alpha* word) {
    return;
}

cell_t* get_data(dict_t *dict, alpha* word) {
    unsigned long hash = hash_code(word);
    size_t index = hash % dict->size;
    return NULL;
}


unsigned long hash_code(alpha* word) {
    unsigned long hash = 5381;
    unsigned long g;
    while (*word != '\0') {
        hash = (hash << 4) + (*word);
        g = hash;
        if (g & 0xf0000000)   {
            hash = hash ^ (g >> 24);
            hash = hash ^ g;
        }
        word++;
    }
    return hash;
}


void file_handle(char* filename) {

    int fd;
    long fsize, reading_size;
    alpha *buffer; 
    alpha word[128];
    size_t idx = 0;
    unsigned long hash;



    fd = open(filename, O_RDONLY);
    if (fd == -1)
        err_sys("Ошибка открытия файла: %s", filename);

    fsize = get_file_size(fd);
    if (fsize == -1) {
        err_sys("Ошибка при определении размера файла: %s", filename);
    }
    

    if (fsize < BUF_SIZE) {
        reading_size = fsize * sizeof(alpha);
        buffer = (alpha*) malloc(reading_size);
    }
    else {
        reading_size = BUF_SIZE * sizeof(uint8_t);
        buffer = (alpha*) malloc(reading_size);
    }

    
    if (buffer == NULL)
        err_sys("Ошибка распределения памяти.");
        
    while ((reading_size = read(fd, buffer, reading_size)) > 0) {
        for (int i = 0; i < reading_size; i++) {
            //printf(" %X\n", buffer[i]);
            if ((buffer[i] >> 7) & 0x01) {
                word[idx++] = buffer[i];
                continue;
            }
            if (isalpha(buffer[i]) || buffer[i] == '\'') {  
                word[idx++] = buffer[i];
            }
            else {
                if (idx > 0)                 {
                    word[idx] = '\0';
                    hash = hash_code(word);
                    printf("hash: %lu,  index: %lu, size: %lu, word: %s\n", 
                            hash, hash % 255, strlen((char*) word), word);
                    idx = 0;    
                }
            }
        }
    }
    free(buffer);
    close(fd);
    return;
}
#include "common.h"
#include "dict.h"
#include <fcntl.h>
#include <stdint.h>
#include <ctype.h>
#include <locale.h>

static const float DEFAULT_FACTOR = 0.72f;
static const size_t DEFAULT_SIZE = 256;
static const float MULT = 2.0f;

static cell_t* create_cell(alpha* key, void* val, val_type_t val_type) {
    cell_t* cell = (cell_t*) alloc(sizeof(cell_t));
    cell->key = (alpha*) alloc(strlen((char*) key));
    memcpy(cell->key, key, strlen((char*) key));

    


    return cell;    
} 

static void destroy_cell(cell_t** cell){
    free((*cell)->key);
    free((*cell)->value);
    free((*cell));
} 


dict_t* create_dict(size_t size, float factor, float mult, val_type_t val_type) {
    
    dict_t *dict = (dict_t*) malloc(sizeof(dict_t));
    if (dict == NULL)
         err_sys("Ошибка распределения памяти: ");

    dict->size = (size >= DEFAULT_SIZE) ? size : DEFAULT_SIZE;
    dict->data = (cell_t**) calloc(dict->size, sizeof(cell_t*));                 
    if (dict->data == NULL)
         err_sys("Ошибка распределения памяти: ");

    dict->count = 0;
    dict->val_type = val_type;
    dict->factor = (factor >= DEFAULT_FACTOR && factor < 1.0f ) 
                    ? factor : DEFAULT_FACTOR;
    dict->limit = (size_t) (dict->factor * dict->size);
    dict->mult = (mult >= MULT) ? mult : MULT;
    
    return dict;
}

void destroy_dict(dict_t* dict) {
    for (size_t i = 0; i < dict->size; i++)  {
        printf("адрес ячейки %lu - %p\n", i, (void*) dict->data[i]); 
        if (dict->data[i]) {
            destroy_cell(&(dict->data[i]));
        }
    }
    free(dict->data);
    free(dict);    
}

static void _put(dict_t** _dict, cell_t *cell) {
    dict_t *dict = *_dict;
    unsigned long hash = hash_code(cell->key);
    size_t index = hash % dict->size;
    if (dict->count < dict->limit) {
        if (dict->data[index] == NULL) {
            dict->data[index] = cell;
        }
        else {
            while (dict->data[index] != NULL)  {
                index++;
                if (index >= dict->size)   {
                    index = 0;
                }
            }
            dict->data[index] = cell;
        }
        dict->count++;
    }
    else {
        printf("Словарь заполнен!!\n");
    }
}

void put(dict_t **_dict, alpha* key, void* value) {
    dict_t *dict = *_dict;

    
    if (cell != NULL) {
        printf("Нашли\n");
    }
    else {
        cell = create_cell();
        printf("Не нашли, ");
        _put(_dict, cell);
    }
}

cell_t* get_data(dict_t *dict, alpha* key) {
    unsigned long hash = hash_code(key);
    size_t index = hash % dict->size;
    cell_t* result = NULL;
    if (dict->data[index] != NULL)  {
        if (CELL_CMP_EQ(dict->data[index]->key, key)) {
            result = dict->data[index];
        }
        else {
            while(dict->data[index] != NULL || 
                    !CELL_CMP_EQ(dict->data[index]->key, key)) {
                index++;
                if (index >= dict->size) {
                    index = 0;
                }
            }
            result = dict->data[index];
        }
    }
    return result;
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
    alpha *w;
    dict_t *dict;



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

    dict = create_dict(256, 0.72f, 2.0f);
        
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
                    /*hash = hash_code(word);
                    w = (alpha*) malloc(strlen((char*) word));
                    memcpy(w, word, strlen((char*) word));
                    printf("hash: %lu,  index: %lu, size: %u, word: %s\n", 
                            hash, hash % 255, CELL_CMP_EQ(word, w), w);*/
                    printf("Прочитали слово %s - \n", word);         
                    //put(&dict, word);
                    //printf("%s\n", word);         
                    idx = 0;    
                }
            }
        }
    }
    /*for (size_t i = 0; i < dict->size; i++) {
        if (dict->data[i])  {
            printf("word: %s - %lu\n", dict->data[i]->word, dict->data[i]->count);
        }
    }*/
    
    destroy_dict(dict);
    free(buffer);
    close(fd);
    return;
}
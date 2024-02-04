#include "common.h"
#include "dict.h"
#include <fcntl.h>
#include <stdint.h>
#include <ctype.h>
#include <locale.h>

static const float DEFAULT_FACTOR = 0.72f;
static const size_t DEFAULT_SIZE = 256;
static const float MULT = 2.0f;

static int recreate = 0;

static cell_t* create_cell(alpha* key, long* val) {
    cell_t* cell = (cell_t*) alloc(sizeof(cell_t));
    cell->key = (alpha*) alloc(strlen((char*) key));
    memcpy(cell->key, key, strlen((char*) key));

    cell->value = (long*) alloc(sizeof(long));
    *cell->value = *val;
    return cell;
}

static void destroy_cell(cell_t** cell){
    free((*cell)->key);
    free((*cell)->value);
    free((*cell));
}


dict_t* create_dict(size_t size, float factor, float mult) {

    dict_t *dict = (dict_t*) malloc(sizeof(dict_t));
    if (dict == NULL)
         err_sys("Ошибка распределения памяти: ");

    dict->size = (size >= DEFAULT_SIZE) ? size : DEFAULT_SIZE;
    dict->data = (cell_t**) calloc(dict->size, sizeof(cell_t*));
    if (dict->data == NULL)
         err_sys("Ошибка распределения памяти: ");

    dict->count = 0;
    dict->factor = (factor >= DEFAULT_FACTOR && factor < 1.0f )
                    ? factor : DEFAULT_FACTOR;
    dict->limit = (size_t) (dict->factor * dict->size);
    dict->mult = (mult >= MULT) ? mult : MULT;

    return dict;
}


void destroy_dict(dict_t* dict) {
    for (size_t i = 0; i < dict->size; i++)  {
        if (dict->data[i]) {
            destroy_cell(&(dict->data[i]));
        }
    }
    free(dict->data);
    free(dict);
}

dict_t* _recreate_dict(dict_t ** _dict, cell_t* cell) {
    printf("начали пересоздавать dict\n");
    dict_t *dict = (*_dict);
    dict_t *new_dict = create_dict((size_t)dict->size * dict->mult,
                                dict->factor, dict->mult);
    print_dict_st(new_dict);
    size_t size = dict->size;
    for(size_t i = 0; i < size; i++) {
        if(dict->data[i] != NULL) {
            _put(&new_dict, dict->data[i]);
        }
    }
    free(dict->data);
    free(*_dict);
    _put(&new_dict, cell);
    *_dict = new_dict;
    printf("Пересоздали dict\n");
    //print_dict_st(*_dict);
    return new_dict;
}

void _put(dict_t** _dict, cell_t *cell) {
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
    }
    else {
        *_dict = _recreate_dict(_dict, cell);
        recreate++;
        
    }
    (*_dict)->count++;
}

void put(dict_t **_dict, alpha* key, long* value) {
    //dict_t *dict = *_dict;
    cell_t *cell = create_cell(key, value);
    _put(_dict, cell);
}

long* get(dict_t *dict, alpha* key) {
    
    unsigned long hash = hash_code(key);
    size_t index = hash % dict->size;
    long* result = NULL;

    if (recreate > 0)
    {
        //printf("%s , %ld\n", dict->data[index]->key, *(dict->data[index]->value));
        //printf("%s \n", dict->data[index]->key);
        printf("хотим получить %s\n", key);    
        printf("hash =  %lu, index =  %lu\n", hash, index);  
        for (size_t i = 0; i < dict->size; i++)
        {
            printf("index %lu, dict: %p\n", i, (void*)dict->data[i]);  
        }
          
    }
    
    
    if (dict->data[index] != NULL)  {
        if (recreate > 0)
        {
            printf("%s \n", dict->data[index]->key);
        }
        if (CELL_CMP_EQ(dict->data[index]->key, key)) {
            
            result = dict->data[index]->value;
        }
        else {
            for(size_t idx = 0; idx < dict->size; idx++) {
                if (dict->data[idx] != NULL && CELL_CMP_EQ(dict->data[idx]->key, key)) {
                    result = dict->data[idx]->value;
                    break;
                }
            }
        }
    }
    if (recreate > 0)
    {
        printf("получили %p\n", (void*)result);    
    }
    return result;
}

unsigned long hash_code(alpha* word) {
    unsigned long hash = 5381;
    int c;
    /*unsigned long g;
    while (*word != '\0') {
        hash = (hash << 5) + (*word);
        g = hash;
        if (g & 0xf0000000)   {
            hash = hash ^ (g >> 24);
            hash = hash ^ g;
        }
        word++;
    }*/
    while (c = *word++) {
        hash = ((hash << 5) + hash) ^ c;
    }
    return hash;
}

void print_dict_st(dict_t* dict) {
    printf("\nФактор загрузки: %f\n", dict->factor);
    printf("На сколько умножаем: %f\n", dict->mult);
    printf("Размер словаря: %lu\n", dict->size);
    printf("Лимит словаря: %lu\n", dict->limit);
    printf("Количество элементов: %lu\n\n", dict->count);
}

void file_handle(char* filename) {

    int fd;
    long fsize, reading_size;
    alpha *buffer;
    alpha word[128];
    size_t idx = 0, vcount = 0;
    long count = 1, hash;
    void *value;
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
    print_dict_st(dict);

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
                    printf(" %s ", word);
                    value = get(dict, word);
                    /*if (recreate > 0) {
                        print_dict_st(dict);
                        printf(" %s ", word);
                        printf("из словаря %p\n", value);
                    }*/
                                        
                    if (value == NULL)
                        put(&dict, word, &count);
                    else {
                       (*((long*)value))++;
                    }
                    //printf("%s\n", word);
                    idx = 0;
                }
            }
        }
    }
    /*for (size_t i = 0; i < dict->size; i++) {
        if (dict->data[i])  {
            printf("word: %s - %lu\n", dict->data[i]->key, *((long*)dict->data[i]->value));
        }
    }*/

    destroy_dict(dict);
    free(buffer);
    close(fd);
    return;
}

#include "common.h"
#include "dict.h"
#include <stdint.h>
#include <ctype.h>
#include <locale.h>

static const float DEFAULT_FACTOR = 0.72f;
static const size_t DEFAULT_SIZE = 256;
static const float MULT = 2.0f;

static cell_t* create_cell(alpha* key, void* val, val_type_t val_type) {
    size_t len = strlen((char*) key);
    cell_t* cell = (cell_t*) alloc(sizeof(cell_t));
    cell->key = (alpha*) alloc(len + 1);
    memcpy(cell->key, key, len);
    cell->key[len] = '\0';
    switch (val_type)
    {
        case CHAR:
            cell->value = (char*) alloc(sizeof(char));
            *((char*)cell->value) = *((char*) val);
            break;
        case INT:
            cell->value = (int*) alloc(sizeof(int));
            *((int*)cell->value) = *((int*) val);
            break;
        case LONG:
            cell->value = (long*) alloc(sizeof(long));
            *((long*)cell->value) = *((long*) val);
            break;
        case FLOAT:
            cell->value = (float*) alloc(sizeof(float));
            *((float*)cell->value) = *((float*) val);
            break;
        case DOUBLE:
            cell->value = (double*) alloc(sizeof(double));
            *((double*)cell->value) = *((double*) val);
            break;
        case STR:
            size_t len = strlen((char*) val);
            alpha* buf = (alpha*) alloc(len + 1);
            memcpy(buf, val, len);
            buf[len] = '\0';
            cell->value = (void*) buf;
            break;
        default:
            err_sys("Не установленный тип значения.");
            break;
    }
    return cell;
}

static void destroy_cell(cell_t** cell){
    free((*cell)->key);
    free((*cell)->value);
    free((*cell));
}


dict_t* create_dict(size_t size, float factor, float mult, val_type_t val_type) {

    dict_t *dict = (dict_t*) alloc(sizeof(dict_t));

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
        if (dict->data[i]) {
            destroy_cell(&(dict->data[i]));
        }
    }
    free(dict->data);
    free(dict);
}

dict_t* _recreate_dict(dict_t ** _dict, cell_t* cell) {
    dict_t *dict = (*_dict);
    dict_t *new_dict = create_dict((size_t)dict->size * dict->mult,
                                dict->factor, dict->mult, dict->val_type);
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
            printf("Индексы совпали %s\n", cell->key);
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
    }
    (*_dict)->count++;
}

void put(dict_t **_dict, alpha* key, void* value) {
    dict_t *dict = *_dict;
    cell_t *cell = create_cell(key, value, dict->val_type);
    _put(_dict, cell);
}

void* get(dict_t *dict, alpha* key) {
    unsigned long long hash = hash_code(key);
    size_t index = hash % dict->size;
    void* result = NULL;
    if (dict->data[index] != NULL)  {
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
    return result;
}

unsigned long long hash_code(alpha* word) {
    unsigned long long hash = 5381;
    /*unsigned long g;
    while (*word != '\0') {
        hash = (hash << 5) + (*word);
        g = hash;
        if (g & 0xf0000000)   {
            hash = hash ^ (g >> 24);
            hash = hash ^ g;
        }
        word++;
    }
    return hash;*/

    int c;
    while ((c = *word++)) {
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

void print_dict(dict_t* dict) {
    size_t index;
    for (size_t i = 0; i < dict->size; i++) {
        if (dict->data[i])  {
            index = (size_t )hash_code(dict->data[i]->key) % dict->size;
            printf("Индекс: %lu, Ключ: %s - значение: ", index, dict->data[i]->key);
            switch (dict->val_type)
            {
                case CHAR:
                    printf("%c\n", *((char*)dict->data[i]->value));
                    break;
                case INT:
                    printf("%d\n", *((int*)dict->data[i]->value));
                    break;
                case LONG:
                    printf("%ld\n", *((long*)dict->data[i]->value));
                    break;
                case FLOAT:
                    printf("%f\n", *((float*)dict->data[i]->value));
                    break;
                case DOUBLE:
                    printf("%f\n", *((double*)dict->data[i]->value));
                    break;
                case STR:
                    printf("%s\n", (alpha*)dict->data[i]->value);
                    break;
                default:
                    printf("\n");
                    break;
            }
        }
    }
}


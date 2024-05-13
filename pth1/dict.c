/*
    Реализация функций работы со словарём
    Словарь dict принимает в качестве ключа строковое значение
    в качестве значения принимается значение в зависимости от val_type_t
*/

#include "log.h"
#include "dict.h"
#include "utils.h"
#include <stdint.h>
#include <ctype.h>
#include <locale.h>

static const float DEFAULT_FACTOR = 0.72f;
static const size_t DEFAULT_SIZE = 256;
static const float MULT = 2.0f;

static cell_t* _create_cell(unsigned char* key, long val);
static void _destroy_cell(cell_t** cell);
static void _put(dict_t** _dict, cell_t *cell);
static cell_t* _get(dict_t* _dict, unsigned char* key);

/*
    функция создания записи словаря:
    cell->key:    ключ словаря
    cell->vvalue: значение ключа
*/

static cell_t* _create_cell(unsigned char* key, long val) {
    size_t len = strlen((char*) key);
    cell_t* cell = (cell_t*) alloc(sizeof(cell_t));
    cell->key = (unsigned char*) alloc(len + 1);
    memcpy(cell->key, key, len);
    cell->key[len] = '\0';
    cell->value = val;
    return cell;
}

//удаление ячейки словаря
static void _destroy_cell(cell_t** cell){
    free((*cell)->key);
    free((*cell));
}

/*
    Процедура созхдания словаря

    size_t size            всего элементов массива
    size_t limit;          если элементов больше чем лимит пересоздаём dict
    float factor;          степень заполнения массива по достижении которого пересоздётся dict
    float mult;             во столько раз увеличится размер dict
    val_type_t val_type;    тип значения ключа
*/

dict_t* create_dict(size_t size, float factor, float mult) {

    dict_t *dict = (dict_t*) alloc(sizeof(dict_t));

    dict->size = (size >= DEFAULT_SIZE) ? size : DEFAULT_SIZE;
    dict->data = (cell_t**) calloc(dict->size, sizeof(cell_t*));
    
    dict->count = 0;
    dict->factor = (factor >= DEFAULT_FACTOR && factor < 1.0f )
                    ? factor : DEFAULT_FACTOR;
    dict->limit = (size_t) (dict->factor * dict->size);
    dict->mult = (mult >= MULT) ? mult : MULT;

    return dict;
}

// удаление словаря

void destroy_dict(dict_t* dict) {
    for (size_t i = 0; i < dict->size; i++)  {
        if (dict->data[i]) {
            _destroy_cell(&(dict->data[i]));
        }
    }
    free(dict->data);
    free(dict);
}

/*
    Процедура пересоздания словаря
*/
dict_t* recreate_dict(dict_t ** _dict, cell_t* cell) {
    dict_t *dict = (*_dict);
    dict_t *new_dict = create_dict((size_t)dict->size * dict->mult,
                                dict->factor, dict->mult);
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

/*
     Положить элемент в словарь
     Вызывается если элемента с данным ключем в словаре нет
*/

static void _put(dict_t** _dict, cell_t *cell) {
    dict_t *dict = *_dict;
    unsigned long long hash = hash_code(cell->key);
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
        *_dict = recreate_dict(_dict, cell);
    }
    (*_dict)->count++;
}

/*
    Положить ключ и значение в словарь.
*/

void put(dict_t **_dict, unsigned char* key, long value) {
    dict_t *dict = *_dict;
    cell_t *cell = _get(dict, key);
    if (cell) {
        cell->value = value;
    }
    else {
        cell = _create_cell(key, value);
        _put(_dict, cell);
    }
}

/*
    найти элемент словаря по ключу
*/

static cell_t* _get(dict_t* dict, unsigned char* key) {

    size_t idx = 0;
    unsigned long long hash = hash_code(key);
    size_t index = hash % dict->size;
    cell_t* result = NULL;
    if (dict->data[index] != NULL)  {
        if (CELL_CMP_EQ(dict->data[index]->key, key)) {
            result = dict->data[index];
        }
        else {
            for(idx = 0; idx < dict->size; idx++) {
                if (dict->data[idx] != NULL && CELL_CMP_EQ(dict->data[idx]->key, key)) {
                    result = dict->data[idx];
                    break;                        
                }
            }
        }
    }
    return result;
}

/*
    найти значение по ключу
*/
int get(dict_t *dict, unsigned char* key, long* value) {

    cell_t * cell = _get(dict, key);
    int result = 0;
    if(cell) {
        *value = cell->value;
        result = 1;
    }
    return result;
}

/*
    процедура вычисления хэш-кода
*/

unsigned long long hash_code(unsigned char* word) {
    unsigned long long hash = 5381;
    int ch;
    while ((ch = *word++)) {
        hash = ((hash << 5) + hash) ^ ch;
    }
    return hash;
}

unsigned long long hashU(unsigned char* word, long m) {
    unsigned long long hash, a = 31415,  b = 27183;
    for (hash = 0; *word != '\0'; word++, a = (a * b) % (m - 1))
        hash = (a * hash + *word) % m;
    return hash;

}

/*
    печать словаря
*/

void print_dict(dict_t* dict) {

    for (size_t i = 0; i < dict->size; i++) {
        if (dict->data[i])  {
            printf("Ключ: %s - значение: %ld\n", dict->data[i]->key, dict->data[i]->value);
        }
    }
}

void get_dict_elements(dict_t *dict, unsigned char** keys, long* values) {
    size_t index = 0;
    for (size_t i = 0; i < dict->size; i++) {
        if (dict->data[i])  {
            keys[index] = dict->data[i]->key;
            values[index] = dict->data[i]->value;
            index++;
        }
    }
}

void print_dict_st(dict_t* dict) {
    printf("\nФактор загрузки: %f\n", dict->factor);
    printf("На сколько умножаем: %f\n", dict->mult);
    printf("Размер словаря: %lu\n", dict->size);
    printf("Лимит словаря: %lu\n", dict->limit);
    printf("Количество элементов: %lu\n\n", dict->count);
}

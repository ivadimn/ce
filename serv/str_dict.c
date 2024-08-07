/*
    Реализация функций работы со словарём
    Словарь dict принимает в качестве ключа строковое значение
    в качестве значения принимается значение в зависимости от val_type_t
*/

#include "log.h"
#include "utils.h"
#include "str_dict.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

static const float DEFAULT_FACTOR = 0.72f;
static const size_t DEFAULT_SIZE = 256;
static const float MULT = 2.0f;

static cell_t* _create_cell(alpha* key, alpha* val);
static void _destroy_cell(cell_t** cell);
static void _put(dict_t** _dict, cell_t *cell);
static cell_t* _get(dict_t* _dict, alpha* key);
static void _set_cell_value(cell_t* cell, alpha* val);

/*
    Процедура установки значения ключа
*/
static void _set_cell_value(cell_t* cell, alpha* val) {

    if(cell->value)
        free(cell->value);
    size_t len = strlen((char*) val);
    alpha* buf = (alpha*) alloc(len + 1);
    memcpy(buf, val, len);
    buf[len] = '\0';
    cell->value = buf;
}
/*
    функция создания записи словаря:
    cell->key:    ключ словаря
    cell->vvalue: значение ключа
*/

static cell_t* _create_cell(alpha* key, alpha* val) {
    size_t len = strlen((char*) key);
    cell_t* cell = (cell_t*) alloc(sizeof(cell_t));
    cell->key = (alpha*) alloc(len + 1);
    memcpy(cell->key, key, len);
    cell->key[len] = '\0';
    cell->value = NULL;
    _set_cell_value(cell, val);
    return cell;
}

//удаление ячейки словаря
static void _destroy_cell(cell_t** cell){
    free((*cell)->key);
    free((*cell)->value);
    free((*cell));
}

/*
    Процедура созхдания словаря

    size_t size            всего элементов массива
    size_t limit;          если элементов больше чем лимит пересоздаём dict
    float factor;          степень заполнения массива по достижении которого пересоздётся dict
    float mult;             во столько раз увеличится размер dict
*/

dict_t* create_dict(size_t size, float factor, float mult) {

    dict_t *dict = (dict_t*) alloc(sizeof(dict_t));

    dict->size = (size >= DEFAULT_SIZE) ? size : DEFAULT_SIZE;
    dict->data = (cell_t**) calloc(dict->size, sizeof(cell_t*));
    if (dict->data == NULL)
         crit("Ошибка распределения памяти: ");

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
void put(dict_t **_dict, alpha* key, alpha* value) {
    dict_t *dict = *_dict;
    cell_t *cell = _get(dict, key);
    if (cell) {
        _set_cell_value(cell, value);
    }
    else {
        cell = _create_cell(key, value);
        _put(_dict, cell);
    }
}

/*
    найти элемент словаря по ключу
*/

static cell_t* _get(dict_t* dict, alpha* key) {

    unsigned long long hash = hash_code(key);
    size_t index = hash % dict->size;
    cell_t* result = NULL;
    if (dict->data[index] != NULL)  {
        if (CELL_CMP_EQ(dict->data[index]->key, key)) {
            result = dict->data[index];
        }
        else {
            for(size_t idx = 0; idx < dict->size; idx++) {
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
void* get(dict_t *dict, alpha* key) {

    cell_t * cell = _get(dict, key);
    void* result = NULL;
    if(cell)
        result = cell->value;
    return result;
}

/*
    процедура вычисления хэш-кода
*/

unsigned long long hash_code(alpha* word) {
    unsigned long long hash = 5381;
    int ch;
    while ((ch = *word++)) {
        hash = ((hash << 5) + hash) ^ ch;
    }
    return hash;
}

/*
    печать словаря
*/

void print_dict(dict_t* dict) {

    for (size_t i = 0; i < dict->size; i++) {
        if (dict->data[i])  {
            printf("Ключ: %s - значение: %s\n", dict->data[i]->key, dict->data[i]->value);
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

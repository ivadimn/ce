#ifndef _DICT_H_
#define _DICT_H_

#include <stddef.h>

#define CELL_CMP_EQ(a, b)  (strcmp((char*)(a), (char*)(b)) == 0)

typedef struct cell_t {
    unsigned char* key;
    long value;
} cell_t;

typedef struct dict_t {
    cell_t **data;          //данные
    size_t count;           //сколько уже заполнено
    size_t size;            //всего элементов массива
    size_t limit;           //если элементов больше чем лимит пересоздаём dict
    float factor;           //степень заполнения массива по
                            //достижении которого пересоздётся dict
    float mult;             //во столько раз увеличится размер dict
} dict_t;


dict_t* create_dict(size_t, float, float);
void destroy_dict(dict_t* dict);

dict_t* recreate_dict(dict_t ** , cell_t*);
void put(dict_t**, unsigned char*, long);
int get(dict_t *dict, unsigned char* word, long* value);
void print_dict_st(dict_t*);
void print_dict(dict_t* );
void get_dict_elements(dict_t *dict, unsigned char** keys, long* values);

unsigned long long hash_code(unsigned char* word);
unsigned long long hashU(unsigned char* word, long m);

#endif

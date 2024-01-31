#ifndef _DICT_H_
#define _DICT_H_

#include <unistd.h>

typedef uint8_t alpha;

typedef struct cell_t {
    size_t count;
    alpha* word;
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
void put(dict_t **dict, alpha* word);
cell_t* get_data(dict_t *dict, alpha* word);


unsigned long hash_code(alpha* word);
void file_handle(char* filename);



#endif
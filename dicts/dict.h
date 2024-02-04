#ifndef _DICT_H_
#define _DICT_H_

#define CELL_CMP_EQ(a, b)  (memcmp((a), (b), strlen((char*)(a))) == 0)
typedef unsigned char alpha;

typedef enum {CHAR, INT, LONG, FLOAT, DOUBLE, STR } val_type_t;

typedef struct cell_t {
    alpha* key;
    long* value;
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

dict_t* _recreate_dict(dict_t ** , cell_t*);
void _put(dict_t** , cell_t* );
void put(dict_t**, alpha*, long*);
long* get(dict_t *dict, alpha* word);
void print_dict_st(dict_t*);


unsigned long hash_code(alpha* word);
void file_handle(char* filename);



#endif

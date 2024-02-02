#ifndef _DICT_H_
#define _DICT_H_

#define CELL_CMP_EQ(a, b)  (memcmp((a), (b), strlen((char*)(a))) == 0)
typedef unsigned char alpha;

typedef enum {CHAR, INT, LONG, FLOAT, DOUBLE, STR } val_type_t;

typedef struct cell_t {
    alpha* key;
    void* value;
} cell_t;

typedef struct dict_t {
    cell_t **data;          //данные
    size_t count;           //сколько уже заполнено
    size_t size;            //всего элементов массива
    size_t limit;           //если элементов больше чем лимит пересоздаём dict
    float factor;           //степень заполнения массива по 
                            //достижении которого пересоздётся dict
    float mult;             //во столько раз увеличится размер dict  
    val_type_t val_type;         
} dict_t;

//static cell_t* create_cell(alpha* word); 
//static void destroy_cell(cell_t* cell); 

dict_t* create_dict(size_t, float, float, val_type_t);
void destroy_dict(dict_t* dict);

//static void _put(dict_t **dict, cell_t* cell);

void put(dict_t **, alpha*, void*);
cell_t* get_data(dict_t *dict, alpha* word);


unsigned long hash_code(alpha* word);
void file_handle(char* filename);



#endif
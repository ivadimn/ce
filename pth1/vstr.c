#include "vstr.h"
#include "utils.h"

vstr_t* vstr_create(long size) {
    vstr_t* str = (vstr_t*) malloc(sizeof(vstr_t));
    if (str == NULL)
        return NULL;
    str->data = (uint8_t*)malloc(sizeof(uint8_t) * (size + 1));
    if (str->data == NULL) {
        free(str);
        return NULL;
    }
    memset(str->data, 0, sizeof(uint8_t) * (size + 1));
    str->size = size + 1;
    str->length = 0;

    return str;
}

void vstr_free(vstr_t* str) {
    free(str->data);
    free(str);
}

size_t vstr_size(vstr_t* str) {
    return str->size;
}

size_t vstr_len(vstr_t* str) {
    return str->length;
}

void vstr_print(vstr_t* str, FILE* f) {
    fwrite(str->data, sizeof(uint8_t), str->length, f);
    if (f == stdout)
        fwrite("\n", 1, 1, f);
}

void vstr_assign(vstr_t *str, const char* value) {
    uint8_t* tmp = (uint8_t*) value;
    long len = strlen(value);
    if (len < str->size) {
        memcpy(str->data, tmp, len);
        str->data[len] = 0;
        str->length = len;
    }
}

vstr_t* vstr_dup(const char* source) {
    size_t len = strlen(source);
    vstr_t* dest = vstr_create(len);
    if (dest) { 
        memcpy(dest->data, source, len);
        dest->length = len;
    }    
    return dest;
}

vstr_t* vstr_concat(vstr_t* left, vstr_t* right) {
    vstr_t *str = vstr_create(left->length + right->length);
    if (str == NULL)
        return NULL;
    memcpy(str->data, left->data, left->length);
    memcpy(str->data + left->length, right->data, right->length);
    str->length = left->length + right->length;
    str->data[str->length] = 0;
    return str;
}

long vstr_in(vstr_t *str, char ch) {
    for (long i = 0; i < str->length; i++)  {
        if (str->data[i] == ch)
            return i;
    }
    return -1;
}

uint8_t vstr_at(vstr_t *str, long index) {
    if (index < str->length)
        return str->data[index];
    return 0;    
    
}

void vstr_split(vstr_array_t* arr, vstr_t* str, char delim, vstr_t* g_open, vstr_t* g_close) {
    long len = str->length, index = 0;
    uint8_t* buf = str->data;
    uint8_t in_group = 0;
    long g_index = -1;

    uint8_t *part = (uint8_t*) alloc(sizeof(uint8_t) * MAX_PART);
    
    for (long i = 0; i < len; i++) {
        
        switch (in_group)  {
            case 0:
                if (buf[i] == delim) {
                    part[index] = '\0';
                    vstr_array_adds(arr, (char*)part);
                    index = 0;
                    
                } 
                else if(g_open && (g_index = vstr_in(g_open, buf[i])) >= 0) {
                    in_group = 1;
                }
                else {
                    part[index++] = buf[i];
                }
                break;
            case 1:
                if (buf[i] == vstr_at(g_close, g_index)) {
                    in_group = 0;
                    g_index = -1;
                }
                else {
                    part[index++] = buf[i];
                }
                break;
            default:
                break;
        }

    }
    part[index] = '\0';
    vstr_array_adds(arr, (char*)part);
    free(part);
}

/*
****************************************************************************
* vstr_array functions
*
*****************************************************************************
*/
static int vstr_array_resize(vstr_array_t *arr);


static int vstr_array_resize(vstr_array_t *arr) {
    long newc = arr->size * 2;
    vstr_t** array = (vstr_t**) malloc(sizeof(vstr_t*) * newc);
    if(array == NULL)
        return -1;
    for (long i = 0; i < arr->length; i++)  {
        array[i] = arr->array[i];
    }
    free(arr->array);
    arr->size = newc;
    arr->array = array;
    return 1;
}

vstr_array_t* vstr_array_create(long size) {
    vstr_array_t *arr = (vstr_array_t*) malloc(sizeof(vstr_array_t));
    if (arr == NULL)
        return NULL;
    arr->array = (vstr_t**) malloc(sizeof(vstr_t*) * size);
    if (arr->array == NULL) {
        free(arr);
        return NULL;
    }
    arr->size = size;
    arr->length = 0;
    return arr;
}

void vstr_array_free(vstr_array_t* arr) {
    for (long i = 0; i < arr->length; i++) {
        free(arr->array[i]);        
    }
    free(arr->array);
    free(arr);
}

void vstr_array_clear(vstr_array_t* arr) {
    for (long i = 0; i < arr->length; i++) {
        free(arr->array[i]);        
    }
    arr->length = 0;
}

long vstr_array_addv(vstr_array_t* arr, vstr_t* str) {
    int rc = 0;
    if (arr->length == arr->size) {
        rc = vstr_array_resize(arr);
        if (rc < 0)
            return rc;
    }
    arr->array[arr->length++] = str;
    return arr->length;
}

long vstr_array_adds(vstr_array_t* arr, const char* str) {
    vstr_t* vstr = vstr_dup(str);
    if(vstr == NULL)
        return -1;

    return vstr_array_addv(arr, vstr);    
}

vstr_t* vstr_array_get(vstr_array_t* arr, long index) {
    if (index >= arr->length)
        return NULL;
    return arr->array[index];    
}
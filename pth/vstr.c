#include "vstr.h"

vstr_t* vstr_create(uint8_t size) {
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
    size_t len = strlen(value);
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

void vstr_split(vstr_t* str, char delim, char grp) {
    size_t len = str->length;
    uint8_t* buf = str->data;
    uint8_t part[4096];

    for (size_t i = 0; i < len; i++) {
        
        if ()
        {
            /* code */
        }
        
    }
    
}
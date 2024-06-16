#include "vstr.h"
#include <stdarg.h>

const uint8_t hex_val[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF};

static long inhex(uint8_t ch) {
	const char* hexch = "0123456789ABCDEF";
	for (long i = 0; i < 16; i++)  {
        if ((uint8_t) hexch[i] == ch)
            return i;
    }
    return -1;
}

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

vstr_t* vstr_plus(long count, ...) {
    long len = 0;
    long len_vals[count];
    va_list ap; 
    vstr_t* str = NULL;        
    va_start(ap, count);    
    for(long i = 0; i < count; i++)  {
        len_vals[i] = strlen(va_arg(ap, char*));
        len += len_vals[i];
    }
    va_end(ap); 
    str = vstr_create(len);
    va_start(ap, count);  
    len = 0; 
    for(long i = 0; i < count; i++)  {
        memcpy(str->data + len, va_arg(ap, char*), len_vals[i]);
        len +=  len_vals[i];
    }
    va_end(ap); 
    str->data[len] = '\0'; 
    str->length = len;
    return str;
}

void vstr_free(vstr_t* str) {
    free(str->data);
    free(str);
}

void vstr_clear(vstr_t* str) {
    memset(str->data, 0, str->size);
    str->length = 0;
}

size_t vstr_size(vstr_t* str) {
    return str->size;
}

size_t vstr_len(vstr_t* str) {
    return str->length;
}

void vstr_print(vstr_t* str, FILE* f) {
    fwrite(str->data, sizeof(uint8_t), str->length, f);
    fwrite("\n", 1, 1, f);
}

void vstr_print_data(vstr_t* str, FILE* f) {
    for (long i = 0; i < str->length; i++) {
        fprintf(f, "%x ", str->data[i]);
    }
    fprintf(f, "\n");
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

vstr_t* vstr_append(vstr_t* left, const char* right) {
    vstr_t *str = NULL;
    long len_r = strlen(right);
    long len_l = left->length;
    if (len_l + len_r >= left->size) {
        str = vstr_create(len_l + len_r);
        if (str == NULL)
            return NULL;
        memcpy(str->data, left->data, len_l);
        vstr_free(left);
    } else {
        str = left;
    }
    memcpy(str->data + len_l, right, len_r);
    str->length = len_l + len_r;
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

long vstr_instr(vstr_t *str, char* s) {
    char* tmp = (char*)str->data;
    long len = (long)strlen(s);
    if (str->length < len)
        return -1;
    
    for (long index = 0; index < str->length - len; index++) {
        if (strncmp(&tmp[index], s, (size_t) len) == 0)
            return index;
    }
    return -1;
}

vstr_t* vstr_substr(vstr_t *str, long start, long end) {
    long len = end - start;
    vstr_t* substr = NULL;
    if(len < 1)
        return NULL;
    substr = vstr_create(len);    
    for (long i = 0; i < len; i++) {
        substr->data[i] = str->data[start + i];
    }
    substr->length = len;
    substr->data[len] = '\0';
    return substr;
}

void vstr_split(vstr_array_t* arr, vstr_t* str, char* delim, vstr_t* g_open, vstr_t* g_close) {
    long len = str->length, index = 0;
    uint8_t* buf = str->data;
    uint8_t in_group = 0;
    long g_index = -1;
    int is_delim = 0;
    vstr_t* dlms = vstr_dup(delim);

    uint8_t *part = (uint8_t*) malloc(sizeof(uint8_t) * MAX_PART);
        
    for (long i = 0; i < len; i++) {
        
        switch (in_group)  {
            case 0:
                if (vstr_in(dlms, buf[i]) >= 0) {
                    if (!is_delim) {
                        part[index] = '\0';
                        vstr_array_adds(arr, (char*)part);
                        index = 0;
                        is_delim = 1;                        
                    }
                } 
                else if(g_open && (g_index = vstr_in(g_open, buf[i])) >= 0) {
                    in_group = 1;
                }
                else {
                    part[index++] = buf[i];
                    is_delim = 0;
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
    if (index > 0) {
        part[index] = '\0';
        vstr_array_adds(arr, (char*)part);    
    }
    vstr_free(dlms);
    free(part);
}

void vstr_put_ch(vstr_t *str, char ch) {
    if(str->length < str->size - 1) {
        str->data[str->length] = (uint8_t) ch;
        str->length++;
        str->data[str->length] = '\0';
    }
}

void vstr_urldecode(vstr_t *str) {
    uint8_t buf[4096];
	uint8_t ch;
	state_t state = ONE;
	long index, buf_index = 0;
    int decode = 0;
    long len = str->length;

    for (long i = 0; i < len; i++) {

		if (str->data[i] == '%') {
            decode = 1;
			continue;
        }
		index = inhex(str->data[i]);
		if (index >= 0 && decode) {
			switch (state) {
				case ONE:
					ch = hex_val[index];
					ch <<= 4;
					state = TWO;
					break;
				case TWO:
					ch |= hex_val[index];
					state = ONE;
					buf[buf_index++] = ch;
                    decode = 0;
					break;
				default:
					break;
			}
		}
		else {
			buf[buf_index++] = str->data[i];
		}
	}
	buf[buf_index++] = '\0';
    vstr_assign(str, (char*) buf);
}

void vstr_cut(vstr_t* str, long count, int where) {
    long start, end, index = 0;
    uint8_t* tmp = NULL;
    if (count >= str->length)
        return;
    str->size = str->length - count + 1;
    tmp = (uint8_t*)malloc(sizeof(uint8_t) * (str->length - count + 1));
    if (where == 0) {
        start = count;
        end = str->length;    
    }
    else {
        start = 0;
        end = str->length - count;
    }
    for (long i = start; i < end; i++) {
        tmp[index++] = str->data[i];
    }
    tmp[index] = '\0';
    free(str->data);
    str->data = tmp;
    str->length -= count;
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
void vstr_array_print(vstr_array_t* arr, FILE* f) {
    for (long i = 0; i < arr->length; i++) {
        vstr_print(arr->array[i], f);
    }
}

vstr_t* vstr_array_join(vstr_array_t* arr, char* delim) {
    long len_delim  = strlen(delim);
    long len_str = 0;
    vstr_t* str = NULL;
    for (long i = 0; i < arr->length; i++) {
        len_str += arr->array[i]->length;
    }
    len_str += (len_delim * (arr->length - 1));
    
    str = vstr_create(len_str);
    len_str = 0;
    for (long i = 0; i < arr->length - 1; i++) {
        memcpy(str->data + len_str, arr->array[i]->data, arr->array[i]->length);
        len_str +=  arr->array[i]->length;
        memcpy(str->data + len_str, delim, len_delim);
        len_str +=  len_delim;
    }
    memcpy(str->data + len_str, arr->array[arr->length - 1]->data, 
            arr->array[arr->length - 1]->length);
    len_str += arr->array[arr->length - 1]->length;
    str->length = len_str;
    str->data[len_str] = '\0';
    return str;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <ctype.h>
#include <unistd.h>


#define BUF_SIZE 4096

typedef union {
    char ch_value;
    int int_value;
    long long_value;
    float float_value;
    double double_value;
    unsigned char* str_value;
} value_t;

// auto - static
typedef unsigned char alpha;
typedef enum {CHAR, INT, LONG, FLOAT, DOUBLE, STR} val_type_t;

void* alloc(size_t size) {
    void *p = malloc(size);
    if (p == NULL) {
        printf("Ошибка распределения памяти: ");
        exit(1);
    }
    return p;
}

long get_file_size(int fd) {

	long fsize = 0;
	struct stat fileStatbuff;
	if ((fstat(fd, & fileStatbuff) != 0) || (!S_ISREG(fileStatbuff.st_mode))) {
		fsize = -1;
	}
	else{
		fsize = fileStatbuff.st_size;
	}
	return fsize;
}


void transform(alpha* k, void* v, val_type_t val_type) {
    alpha* key;
    value_t value;
    size_t len = strlen((char*) k);
    key = (alpha*) alloc(len + 1);
    memcpy(key, k, len);
    key[len] = '\0';

    /*switch (val_type)
    {
    case CHAR:
        value = (char*) alloc(sizeof(char));
        *((char*)value) = *((char*) v);
        break;
    case INT:
        value = (int*) alloc(sizeof(int));
        *((int*)value) = *((int*) v);
        break;
    case LONG:
        value = (long*) alloc(sizeof(long));
        *((long*)value) = *((long*) v);
        break; 
    case STR:
        value = (alpha*) alloc(strlen((char*) v));
        strcpy(value, v);
        break;           
    default:
        break;
    }*/

    switch (val_type)
    {
    case CHAR:
        value.ch_value = *((char*) v);
        break;
    case INT:
        value.int_value = *((int*) v);
        break;
    case LONG:
        value.long_value = *((long*) v);
        break; 
    case STR:
        value.str_value = (alpha*) alloc(strlen((char*) v));
        strcpy(value.str_value, v);
        break;           
    default:
        break;
    }

    int eq = memcmp(key, k, len);
    printf("key = %s eq = %d, len = %lu\n", key, eq, len);
    free(key);
    
}

void file_handle(char* filename) {

    int fd;
    long fsize, reading_size;
    alpha *buffer;
    alpha word[128];
    size_t idx = 0;
    long value = 1;
        
    fd = open(filename, O_RDONLY);
    
    if (fd == -1) {
        printf("Ошибка открытия файла: %s\n", filename);
        exit(1);
    }    
    
    fsize = get_file_size(fd);
    if (fsize == -1) {
        printf("Ошибка при определении размера файла: %s\n", filename);
        exit(1);
    }
    
    printf("размер файла %ld\n", fsize);
    if (fsize < BUF_SIZE) {
        reading_size = fsize * sizeof(alpha);
        buffer = (alpha*) alloc(reading_size);
    }
    else {
        reading_size = BUF_SIZE * sizeof(alpha);
        buffer = (alpha*) alloc(reading_size);
    }
    printf("размер чтения %lu, buffer %p\n", reading_size, (void*) buffer);
    while ((reading_size = read(fd, buffer, reading_size)) > 0) {
        
        for (int i = 0; i < reading_size; i++) {
            
            if ((buffer[i] >> 7) & 0x01) {
                word[idx++] = buffer[i];
                continue;
            }
            if (isalpha(buffer[i]) || buffer[i] == '\'') {
                word[idx++] = buffer[i];
            }
            else {
                if (idx > 0)                 {
                    word[idx] = '\0';
                    printf("word:  %s\n", word);
                    transform(word, &value, LONG);
                    idx = 0;
                }
            }
        }
    }
    free(buffer);
    close(fd);
    return;
}

int main()
{
    printf("открыли\n");
    file_handle("utf12510.txt");
    printf("открыли\n");
    return 0;

}
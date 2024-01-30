#include "common.h"
#include "dict.h"
#include <fcntl.h>
#include <stdint.h>
#include <ctype.h>

typedef char alpha;

void file_handle(char* filename) {

    int fd;
    long fsize, reading_size;
    alpha *buffer; 
    alpha word[128];
    size_t idx = 0;

    fd = open(filename, O_RDONLY);
    if (fd == -1)
        err_sys("Ошибка открытия файла: %s", filename);

    fsize = get_file_size(fd);
    if (fsize == -1) {
        err_sys("Ошибка при определении размера файла: %s", filename);
    }
    

    if (fsize < BUF_SIZE) {
        reading_size = fsize * sizeof(alpha);
        buffer = (alpha*) malloc(reading_size);
    }
    else {
        reading_size = BUF_SIZE * sizeof(uint8_t);
        buffer = (alpha*) malloc(reading_size);
    }

    //ispunct(2);

    if (buffer == NULL)
        err_sys("Ошибка распределения памяти.");
        
    while (read(fd, buffer, reading_size) == reading_size) {
        for (int i = 0; i < reading_size; i++) {
           if(ispunct(buffer[i]) || isspace(buffer[i])) {
                if (idx > 0)                 {
                    word[idx] = '\0';
                    printf("Word: %s\n", word);
                    idx = 0;    
                }
           }
           else {
                word[idx++] = buffer[i];
           }
        }
    }
    free(buffer);
    close(fd);
    return;
}
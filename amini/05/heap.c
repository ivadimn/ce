#include <stdio.h>
#include <stdlib.h>

void print_mem_maps() {
#ifdef __linux__
    FILE* fd = fopen("/proc/self/maps", "r");
    if (!fd) {
        printf("Could not open map file.\n");
        exit(1);
    }
    char line[1024];
    while (!feof(fd)) {
        fgets(line, 1024, fd);
        printf("> %s", line);
    }
    fclose(fd);
#endif
}

int main(int argc, char** argv) {
    //выделяем 10 байт без инициализации
    char* ptr1 = (char*) malloc(10 * sizeof(char));
    printf("Address of ptr1: %p\n", (void*) &ptr1);
    printf("Memory allocated by malloc at: %p\n", (void*) ptr1);
    for (int i = 0; i < 10; i++) {
        printf("0x%02x", (unsigned char) ptr1[1]);
    }
    printf("\n");
    //выделяем 10 байт каждый из которых обнулён
    char* ptr2 = (char*) calloc(10, sizeof(char));
    printf("Address of ptr2: %p\n", (void*) &ptr2);
    printf("Memory allocated by calloc at: %p\n", (void*) ptr2);
    for (int i = 0; i < 10; i++) {
        printf("0x%02x", (unsigned char) ptr2[1]);
    }
    printf("\n");
    print_mem_maps();
    free(ptr1);
    free(ptr2);
    
    return 0;
}
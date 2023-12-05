#include <stdlib.h>
#include <stdio.h>

void fill(char* ptr) {
    ptr[0] = 'H';
    ptr[1] = 'e';
    ptr[2] = 'l';
    ptr[3] = 'l';
    ptr[4] = 0;
}


int main(int argc, char** argv) {
    void *gptr = malloc(10 * sizeof(char));
    char *ptr = (char*) gptr;
    fill(ptr);
    printf("%s!\n", ptr);
    free(ptr);

    return 0;
}
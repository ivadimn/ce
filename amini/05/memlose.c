#include <stdlib.h>

int main(int argc, char** argv) {
    char* ptr = (char*) malloc(32 * sizeof(char));
    free(ptr);
    return 0;
}
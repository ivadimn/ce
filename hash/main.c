#include <stdio.h>
#include <string.h>
#include "hash.h"

int main(int argc, char** argv) {
    char str[32];
    while(1) {
        printf("Введите строку символов: ");
        scanf("%s", str);
        if (strcmp("end", str) == 0) {
            break;
        }
        
        int hash = hash_wein(str);
        printf("Hash-значение %s равно %d\n", str, hash);
    }

    return 0;
}
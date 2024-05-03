#include "vstr.h"


vstr_t **array;

int main() {
    char* str  = "Слово по русски";
    vstr_t *sdup = vstr_dup(str);
    vstr_t *s2 = vstr_create(16);
    vstr_assign(s2, "Hello world");
    vstr_t *sc = vstr_concat(sdup, s2);

    vstr_print(sdup, stdout);
    vstr_print(s2, stdout);
    vstr_print(sc, stdout);

    array = (vstr_t**) malloc(sizeof(vstr_t*) * 10);
    array[0] = sdup;
    array[1] = s2;
    array[2] = sc;

    for (size_t i = 0; i < 3; i++) {
        vstr_print(array[i], stdout);
    }
    
    
}
#include "vstr.h"

int main() {
    char* str  = "Слово по русски";
    vstr_t *sdup = vstr_dup(str);
    vstr_t *s2 = vstr_create(16);
    vstr_assign(s2, "Hello world");
    vstr_t *sc = vstr_concat(sdup, s2);

    vstr_print(sdup, stdout);
    vstr_print(s2, stdout);

    fprintf(stdout, "\n length = %ld\n", vstr_len(sc));
    vstr_print(sc, stdout);
    printf("%s\n", sc->data);
}
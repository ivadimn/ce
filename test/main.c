#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "vstr.h"

#define MAX_LEN 64


int main() {
    char buf[MAX_LEN];

    time_t mytime = time(NULL);
    struct tm *now = localtime(&mytime);
    strftime(buf, MAX_LEN - 1, "%a, %d %b %Y %H:%M:%S %Z", now);

    vstr_t* str = vstr_plus(4, "1111", "2222", "3333", "4444");
    vstr_print(str, stdout);
    printf("%s\n", buf);
    printf("len: %ld, size: %ld\n", str->length, str->size);

    vstr_array_t* arr = vstr_array_create(8);
    vstr_array_adds(arr, "1111");
    vstr_array_adds(arr, "2222");
    vstr_array_adds(arr, "3333");
    vstr_array_adds(arr, "4444");

    vstr_array_print(arr, stdout);

    vstr_t* sj = vstr_array_join(arr, "#@");
    vstr_print(sj, stdout);

}
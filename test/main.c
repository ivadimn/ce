#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "vstr.h"
#include "str_dict.h"

#define MAX_LEN 64


int main() {
    char buf[MAX_LEN];
    unsigned char key[16];
    unsigned char value[16];
    time_t mytime = time(NULL);
    struct tm *now = localtime(&mytime);
    strftime(buf, MAX_LEN - 1, "%a, %d %b %Y %H:%M:%S %Z", now);

    vstr_t* str = vstr_create(32);
    str = vstr_append(str, "123456789");
    vstr_print(str, stdout);
    str =  vstr_append(str, "ABCDEFGHIJKLMNOP");
    vstr_print(str, stdout);
    str = vstr_append(str, "109876545321");
    vstr_print(str, stdout);
    str = vstr_append(str, "RGB-RGB-RGB-RGB-RGB");
    vstr_print(str, stdout);
    str = vstr_append(str, "A1B2C3D4E5F6E7F8G9");
    vstr_print(str, stdout);
    vstr_free(str);

}
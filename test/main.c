#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"

#define MAX_LEN 64


int main() {
    char buf[MAX_LEN];
    char arr[3][256];

    time_t mytime = time(NULL);
    struct tm *now = localtime(&mytime);
    strftime(buf, MAX_LEN - 1, "%a, %d %b %Y %H:%M:%S %Z", now);

    long count = split(arr, "GET /url_string HTTP/1.1", ' ', NULL, NULL);
    printf("%s\n", arr[0]);
    printf("%s\n", arr[1]);
    printf("%s\n", arr[2]);

    printf("%s\n", buf);
}
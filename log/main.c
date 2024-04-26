#include "log.h"


int main(int argc, char** argv) {
    
    time_t mytime = time(NULL);
    struct tm *now = localtime(&mytime);
    char str[64];
    strftime(str, sizeof(str), "%c", now);
    printf("Time: %s\n", str);
    "Apr 15 2024 09:13:10";
    return 0;
}
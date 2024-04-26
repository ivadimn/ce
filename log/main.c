#include "log.h"


int main(int argc, char** argv) {
    
    char buf[MAX_LEN];

    time_t mytime = time(NULL);
    struct tm *now = localtime(&mytime);
    strftime(buf, MAX_LEN - 1, "%b %d %Y %H:%M:%S ", now);
    printf("%s\n", buf);
    return 0;
}
#include "log.h"

#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define YELLOW  "\033[1;33m"
#define WHITE   "\033[1;37m"

int main(int argc, char** argv) {
    
    char buf[MAX_LEN];

    time_t mytime = time(NULL);
    struct tm *now = localtime(&mytime);
    strftime(buf, MAX_LEN - 1, "%b %d %Y %H:%M:%S ", now);
    printf("%s\n", buf);
    printf("%s00%s",    YELLOW, RESET);
    printf("%s===3%s ", RED,    RESET);
    printf("%s-.%s\n",  WHITE,  RESET);
    return 0;
}
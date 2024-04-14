#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

static void charatatime(char *str)
{
    char *ptr;
    int c;

    setbuf(stdout, NULL);
    for (ptr = str; (c = *ptr++) != 0; )
        putc(c, stdout);
}

int main()
{
    pid_t pid;

    if ((pid = fork()) < 0) {
        perror("fork");
    } else if (pid == 0) {
        charatatime("from child process\n");
    } else {
        charatatime("from parent process\n");
    }
    exit(0);
}
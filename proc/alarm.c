#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <pwd.h>
#include <unistd.h>

static void my_alarm(int signo)
{
    struct passwd *rootptr;
    printf("внутри обработчика сигнала\n");
    if ((rootptr = getpwnam("root")) == NULL)
        perror("getpwnam");
    alarm(1);
}

static const char* username = "vadim";

int main(void)
{
    struct passwd* ptr;
    signal(SIGALRM, my_alarm);
    alarm(1);
    for (;;) {
        if ((ptr = getpwnam(username)) == NULL)
            perror("getpwnam");
        if (strcmp(ptr->pw_name, username) != 0)
            printf("возвращаемое значение повреждено! pw_name = %s\n",
                    ptr->pw_name);
    }
}
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

static void sig_usr(int signo)
{
    if (signo == SIGUSR1)
        printf("принят сигнал SIGUSR1\n");
    else if (signo == SIGUSR2)
        printf("принят сигнал SIGUSR2\n");
    else
        printf("принят сигнал %d\n", signo);
}

int main()
{
    if (signal(SIGUSR1, sig_usr) == SIG_ERR)
        perror("signal SIGUSR1");
    if (signal(SIGUSR2, sig_usr) == SIG_ERR)
        perror("signal SIGUSR2");
    for(;;)
        pause();
}
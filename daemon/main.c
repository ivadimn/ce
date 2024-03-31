#include "common.h"
#include <sys/types.h>
#include <sys/wait.h>



int globvar = 6;
char buf[] = "запись в stdout\n";

void pr_exit(int status) {
    if (WIFEXITED(status))  {
        printf("нормальное завершение, код выхода = %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("аварийное завершение, код выхода = %d%s\n", WTERMSIG(status),
#ifdef WCOREDUMP
            WCOREDUMP(status) ? "(создан файл core)" : ""); 
#else
            "");
#endif    
    }
    else if (WIFSTOPPED(status)) 
        printf("дочерний процесс остановлен, номер сигнала = %d\n", 
                WSTOPSIG(status));
}

void charatatime(char* str) {
    char *ptr;
    int c;
    
    setbuf(stdout, NULL);      //небуферизированный режим вывода
    for (ptr = str; (c = *ptr++) != 0; ) {
        putc(c, stdout);
    }
}

int main(void) {
    
    pid_t pid;

    if ((pid = fork()) < 0) {
        err_sys("Ошибка вызова функции fork");
    } else if (pid == 0) {
        charatatime("от дочернего процесса\n");
    } else {
        charatatime("от родительского процесса\n");
    }


    /*if ((pid = fork()) < 0) {
        err_sys("Ошибка вызова функции fork");
    } else if (pid == 0) {     //первый потомок
        if ((pid = fork()) < 0)
            err_sys("Ошибка вызова функции fork");
        else if (pid > 0)                     
            exit(0);            //первый потомок он же родительский 
                                //процесс для для второго потомка
        */                        
        /* 
        *   Здесь продолжает работу второй потомок, для котрого родительским 
        *   стал процесс init, поскольку настоящий родительский процесс
        *   вызвал функцию exit() чуть выше
        *   Теперь можно продолжить работу, зная, что когда процесс завершится
        *   его код завершения получит процесс init 
        */
        /*sleep(2);
        printf("второй потомок, идентифйикатор родительского процесса = %ld\n",
                (long)getppid());
        exit(0);        
    }
    if (waitpid(pid, NULL, 0) != pid)
            err_sys("Ошибка вызова функции waitpid");
    */
    /*
    *   Здесь продолжает работу родительский (первоначальный) процесс,
    *   поскольку он не является родительским процессом для второго потомка
    */
    


    /*int status;

    if ((pid = fork()) < 0)
        err_sys("Ошибка вызова функции fork");
    else if (pid == 0)                     //дочерний процесс
        exit(7);
        
    if (wait(&status) != pid) //дождаться завершения дочернего процесса
        err_sys("Ошибка вызова функции wait");
    pr_exit(status);


    if ((pid = fork()) < 0)
        err_sys("Ошибка вызова функции fork");
    else if (pid == 0)                     //дочерний процесс
        abort();
        
    if (wait(&status) != pid) //дождаться завершения дочернего процесса
        err_sys("Ошибка вызова функции wait");
    pr_exit(status);


    if ((pid = fork()) < 0)
        err_sys("Ошибка вызова функции fork");
    else if (pid == 0)                     //дочерний процесс
        status /= 0;
        
    if (wait(&status) != pid) //дождаться завершения дочернего процесса
        err_sys("Ошибка вызова функции wait");
    pr_exit(status); */


    /*int var;

    var = 88;
    if (write(STDOUT_FILENO, buf, sizeof(buf) - 1) != sizeof(buf) - 1) {
        err_sys("ошибка вызова функции write");
    }
    printf("перед вызовом функции fork\n");

    if ((pid = fork()) < 0) {
        err_sys("ошибка вызова функции fork");
    } else if (pid == 0) {    //дочерний процесс
        globvar++;
        var++;
    } else {
        sleep(2);
    }

    printf("pid = %ld, globvar = %d, var = %d\n", (long) getpid(), globvar, var);*/

    exit(0);
}
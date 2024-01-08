#include "common.h"
#include <errno.h>
#include <stdarg.h>

static void err_doit(int, int, const char*, va_list);

/*
* Обрабатывает нефатальные ошибки, связанные с системными вызовами
* Выводит сообщение и возвращает управление
*/
void err_ret(const char *fmt, ...) {
    
    va_list ap;
    va_start(ap, fmt);
    err_doit(1, errno, fmt, ap);
    va_end(ap);
}

/*
* Обрабатывает фатальные ошибки, связанные с системными вызовами
* Выводит сообщение и завершает работу процесса
*/
void err_sys(const char *fmt, ...) {
    
    va_list ap;

    va_start(ap, fmt);
    err_doit(1, errno, fmt, ap);
    exit(1);
}

/*
* Обрабатывает нефатальные ошибки, не связанные с системными вызовами
* Код ошибки передаётся в аргументе
* Выводит сообщение и возвращает управление
*/
void err_cont(const char *fmt, ...) {
    
    va_list ap;

    va_start(ap, fmt);
    err_doit(1, errno, fmt, ap);
    va_end(ap);
}

/*
* Обрабатывает фатальные ошибки, не связанные с системными вызовами
* Код ошибки передаётся в аргументе
* Выводит сообщение и завершает работу процессу
*/
void err_exit(int error, const char *fmt, ...) {
    
    va_list ap;

    va_start(ap, fmt);
    err_doit(1, error, fmt, ap);
    va_end(ap);
    exit(1);
}

/*
* Обрабатывает фатальные ошибки, связанные с системными вызовами
* Выводит сообщение, создаёт файл core и  завершает работу процесса 
*/
void err_dump(const char *fmt, ...) {
    
    va_list ap;

    va_start(ap, fmt);
    err_doit(1, errno, fmt, ap);
    va_end(ap);
    abort();    //записать дамп памяти в файл и завершить процесс
    exit(1);     //этот вызов никогда не должен выполняться
}

/*
* Обрабатывает нефатальные ошибки, не связанные с системными вызовами
* Выводит сообщение и возвращает управление 
*/
void err_msg(const char *fmt, ...) {
    
    va_list ap;

    va_start(ap, fmt);
    err_doit(0, 0, fmt, ap);
    va_end(ap);
}

/*
* Обрабатывает фатальные ошибки, не связанные с системными вызовами
* Выводит сообщение и завершает работу процесса 
*/
void err_quit(const char *fmt, ...) {
    
    va_list ap;

    va_start(ap, fmt);
    err_doit(0, 0, fmt, ap);
    va_end(ap);
    exit(1);
}

/*
* Выводит сообщение и возвращает управление в вызывающую функцию.
* Вызывающая функция определяет значение флага "errnoflag". 
*/
void err_doit(int errnoflag, int error, const char *fmt, va_list ap) {
    
    char buf[MAXLINE];

    vsnprintf(buf, MAXLINE-1, fmt, ap);
    if (errnoflag)     {
        snprintf(buf + strlen(buf), MAXLINE - strlen(buf) - 1, ": %s",
                strerror(error));
    }
    strcat(buf, "\n");
    fflush(stdout);
    fputs(buf, stderr);
    fflush(NULL);
}

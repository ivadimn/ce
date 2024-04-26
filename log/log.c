#include "log.h"
#include <errno.h>
#include <syslog.h>
#include <stdarg.h>

static void log_paste(int errnoflag, int error, int priority, 
                    const char* fmt, va_list va);

void log_write(int errnoflag, int error, int priority, 
                    const char* fmt, va_list va);


static int log_to_stderr = 1;
static int file = -1;


static const char* log_label(int level) {

     switch(level)
     {
        case LOG_DEBUG:  
            return "DEBUG"; 
        case LOG_INFO:   
            return "INFO"; 
        case LOG_NOTICE: 
            return "NOTICE"; 
        case LOG_WARNING:
            return "WARNING";
        case LOG_ERR:    
            return "ERROR";
        case LOG_CRIT:   
            return "CRITICAL";
        case LOG_ALERT:   
            return "ALERT";
        case LOG_EMERG:   
            return "emg";
        default: 
            return "UNKNOWN";
     }
}


/*
* инициализировать syslog если процесс работает в режиме демона
*/

void log_open(const char* app, int option, int facility)
{
    if(log_to_stderr == 0)
        openlog(app, option, facility);
}

/*
* Обрабатывает нефатальные ошибки, связанные с системными вызовами
* Выводит сообщение, соответствующее содержимому переменной errno
* и возвращает управление 
*/
void log_ret(const char *fmt, ...) {
    
    va_list ap;

    va_start(ap, fmt);
    log_paste(1, errno, LOG_ERR, fmt, ap);
    va_end(ap);
}

/*
* Обрабатывает фатальные ошибки, связанные с системными вызовами
* Выводит сообщение и завершает работу процесса
*/
void log_sys(const char *fmt, ...) {
    
    va_list ap;

    va_start(ap, fmt);
    log_paste(1, errno, LOG_ERR, fmt, ap);
    exit(2);
}

/*
* Выводит информационное сообщение и возвращает управление 
*/
void log_info(const char *fmt, ...) {
    
    va_list ap;

    va_start(ap, fmt);
    log_paste(0, 0, LOG_INFO, fmt, ap);
    va_end(ap);
}

/*
* Обрабатывает нефатальные ошибки, не связанные с системными вызовами
* Выводит сообщение и возвращает управление 
*/
void log_msg(const char *fmt, ...) {
    
    va_list ap;

    va_start(ap, fmt);
    log_paste(0, 0, LOG_ERR, fmt, ap);
    va_end(ap);
}

/*
* Обрабатывает фатальные ошибки, не связанные с системными вызовами
* Выводит сообщение и завершает работу процесса 
*/
void log_quit(const char *fmt, ...) {
    
    va_list ap;
    
    va_start(ap, fmt);
    log_paste(0, 0, LOG_ERR, fmt, ap);
    va_end(ap);
    exit(2);
}

/*
* Обрабатывает фатальные ошибки, не связанные с системными вызовами
* Код ошибки передаётся в аргументе
* Выводит сообщение и возвращает управление
*/
void log_exit(int error, const char *fmt, ...) {
    
    va_list ap;

    va_start(ap, fmt);
    log_paste(1, error, LOG_ERR, fmt, ap);
    va_end(ap);
    exit(2);
}


static void log_paste(int errnoflag, int error, int priority, 
                    const char* fmt, va_list ap) {

    char buf[MAX_LEN];

    vsnprintf(buf, MAX_LEN - 1, fmt, ap);

    if (errnoflag) {
        snprintf(buf + strlen(buf), MAX_LEN - strlen(buf) - 1, ": %s", 
                strerror(error));
    }
    strcat(buf, "\n");
    if (log_to_stderr) {
        fflush(stdout);
        fputs(buf, stderr);
        fflush(stderr);
    } else {
        syslog(priority, "%s", buf);
    }
}

void log_write(int errnoflag, int error, int priority, 
                            const char* fmt, va_list va) {
    
    char buf[MAX_LEN];

    time_t mytime = time(NULL);
    struct tm *now = localtime(&mytime);
    strftime(buf, MAX_LEN - 1, "%b %d %Y %H:%M:%s ", now);
    printf("%s\n", buf);


}

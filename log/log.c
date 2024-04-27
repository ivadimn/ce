#include "log.h"
#include <errno.h>
#include <syslog.h>
#include <stdarg.h>

static void log_paste(int errnoflag, int error, int priority, 
                    const char* fmt, va_list va);

void log_write(int errnoflag, int error, int level, 
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
* инициализировать лог файл
* если эта функция не вызвана
* то логи будут писаться в консоль
*/

void log_open(const char* log_file)
{
    file = open(log_file, O_CREAT | O_APPEND, S_IRWXU);
    if(file < 0){
        log_ret("Не удалось открыть лог-файл.");
        return;
    }
    log_to_stderr = 0;
}

/*
* закрывает лог-файл
*/
void log_close() {
    if(file > 0) {
        close(file);
        log_to_stderr = 0;
    } 
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

void log_write(int errnoflag, int error, int level, 
                            const char* fmt, va_list va) {
    
    char buf[MAX_LEN];
    size_t wrote = 0;

    time_t mytime = time(NULL);
    struct tm *now = localtime(&mytime);

    wrote = strftime(buf, MAX_LEN - 1, "%b %d %Y %H:%M:%S ", now);
    wrote += snprintf(buf + wrote, MAX_LEN - wrote - 1, " %s: ", log_label(level));
    wrote += vsnprintf(buf + wrote, MAX_LEN - wrote - 1, fmt, va);
    if (errnoflag) {
        snprintf(buf + wrote, MAX_LEN - wrote - 1, ": %s", strerror(error));
    }
    strcat(buf, "\n");
    printf("%s\n", buf);


}

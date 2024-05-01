#include "log.h"
#include <errno.h>
#include <pthread.h>


static void log_write(const char* func_name, int errnoflag, int error, 
                            int level, const char* fmt, va_list ap);
static void console(int level, const char *buf);
static void stackframe(char* st_info);


static int log_to_stderr = 1;
static int file = -1;
static pthread_mutex_t mtx;     // для синхронизации записи лога

/*
* Возвращает текстовое описание уровня лога
*/
static const char* log_label(int level) {

     switch(level)
     {
        case LOG_DEBUG:  
            return "DEBUG"; 
        case LOG_INFO:   
            return "INFO"; 
        case LOG_WARNING:
            return "WARNING";
        case LOG_ERR:    
            return "ERROR";
        case LOG_CRIT:   
            return "CRITICAL";
        default: 
            return "UNKNOWN";
     }
}

/*
* сохранить содержимое стэка в буфер st_info
*/
void stackframe(char* st_info) {
    size_t wrote = 0;
    void *trace[32];
    char **messages = (char **)NULL;
    int trace_size = 0;

    trace_size = backtrace(trace, 32);
    messages = backtrace_symbols(trace, trace_size);
    wrote += snprintf(st_info + wrote, MAX_LEN - wrote - 1, "%s\n", "Stack info:");
    for (int i = 2; i < trace_size; ++i)
         wrote += snprintf(st_info + wrote, MAX_LEN - wrote - 1, "... %s\n", messages[i]);
}

/*
* инициализировать лог файл
* если log file == NULL
* то логи будут писаться в консоль
* если эта функция не вызвана, то логи будут писаться в консоль
* и синхронизации записи не будет
*/
void log_open(const char* log_file)
{
    file = open(log_file, O_CREAT | O_APPEND, S_IRWXU);
    if(file < 0){
        err("Не удалось открыть лог-файл.");
        return;
    }
    log_to_stderr = 0;
    pthread_mutex_init(&mtx, NULL);    
}

/*
* закрывает лог-файл
*/
void log_close() {
    if(file > 0) {
        close(file);
        log_to_stderr = 0;
    } 
    pthread_mutex_destroy(&mtx);
}

/*
* Вывод сообщения в консоль
*/
void console(int level, const char *buf) {
    
    fflush(stdout);
    switch(level)
    {
        case LOG_DEBUG:  
            fprintf(stderr, "%s%s%s\n", DEBUG, buf, RESET);
            break; 
        case LOG_INFO:   
            fprintf(stderr, "%s%s%s\n", INFO, buf, RESET);
            break; 
        case LOG_WARNING:
            fprintf(stderr, "%s%s%s\n", WARNING, buf, RESET);
            break; 
        case LOG_ERR:    
            fprintf(stderr, "%s%s%s\n", ERROR, buf, RESET);
            break; 
        case LOG_CRIT:   
            fprintf(stderr, "%s%s%s\n", ERROR, buf, RESET);
            break; 
        default: 
            fprintf(stderr, "%s%s%s\n", INFO, buf, RESET);
    }
    fflush(stderr);
}

/*
* Выводит сообщение в лог
*/
void logv(const char* func_name, int level, const char* fmt, ...) {
    va_list ap;
    pthread_mutex_lock(&mtx);
    va_start(ap, fmt);
    if (level == LOG_CRIT) {
        log_write(func_name, 1, errno, level, fmt, ap);
        va_end(ap);
        abort(); 
        pthread_mutex_unlock(&mtx);
        exit(1);    
    }
    else if (level == LOG_ERR) {
        log_write(func_name, 1, errno, level, fmt, ap);
    }
    else {
        log_write(func_name, 0, 0, level, fmt, ap);
    }
    va_end(ap);
    pthread_mutex_unlock(&mtx);
}


/*
* Выводит информационное сообщение
*/
// void log_info(const char* fmt, ...) {

//     va_list ap;

//     va_start(ap, fmt);
//     log_write(0, 0, LOG_INFO, fmt, ap);
//     va_end(ap);
// }

/*
* Выводит отладочное сообщение
*/
// void log_debug(const char* fmt, ...) {

//     va_list ap;

//     va_start(ap, fmt);
//     log_write(0, 0, LOG_DEBUG, fmt, ap);
//     va_end(ap);
// }

/*
* Выводит предупреждение
*/
// void log_warning(const char* fmt, ...) {

//     va_list ap;

//     va_start(ap, fmt);
//     log_write(0, 0, LOG_WARNING, fmt, ap);
//     va_end(ap);
// }

/*
* Выводит сообщение об ошибке
*/
// void log_err(const char *fmt, ...) {
    
//     va_list ap;

//     va_start(ap, fmt);
//     log_write(1, errno, LOG_ERR, fmt, ap);
//     va_end(ap);
// }

/*
* Выводит сообщение о критической ошибке и завершает выполнение программы
*/
// void log_crit(const char* fmt, ...) {
//     va_list ap;
//     va_start(ap, fmt);
//     log_write(1, errno, LOG_CRIT, fmt, ap);
//     va_end(ap);
//     abort(); 
//     exit(1);
// }

/*
* записать сообщение в лог файл или вывести на консоль
* в случае LOG_ERROR или LOG_CRIT выводится cтэк вызовов
*/
void log_write(const char* func_name, int errnoflag, int error, int level, 
                            const char* fmt, va_list ap) {
    
    char buf[MAX_LEN];
    char st_info[MAX_LEN];
    size_t wrote = 0;

    time_t mytime = time(NULL);
    struct tm *now = localtime(&mytime);

    wrote = strftime(buf, MAX_LEN - 1, "%b %d %Y %H:%M:%S ", now);
    wrote += snprintf(buf + wrote, MAX_LEN - wrote - 1, " %s [%s]: ", log_label(level), func_name);
    wrote += vsnprintf(buf + wrote, MAX_LEN - wrote - 1, fmt, ap);
    
    if (errnoflag) {
        snprintf(buf + wrote, MAX_LEN - wrote - 1, ": %s", strerror(error));
    }
    if (level == LOG_ERR || level == LOG_CRIT) {
        stackframe(st_info);
    }
    

    if (log_to_stderr) {
        console(level, buf);
        if (level == LOG_ERR || level == LOG_CRIT)
            console(level, st_info);
    } else {
        strcat(buf, "\n");
        write(file, buf, strlen(buf));
        if (level == LOG_ERR || level == LOG_CRIT)
            write(file, st_info, strlen(st_info));
    }

}

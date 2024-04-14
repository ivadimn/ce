#include "common.h"
#include "turn_daemon.h"
#include <stdlib.h>
#include <errno.h>

int turn_daemon(const char* cmd) {
    int fd;
    struct rlimit rl;
    struct sigaction sa;
    
    
    openlog(cmd, LOG_CONS, LOG_DAEMON);     //инициализация файла журнала

    /*
    * Получить максимально возможный номер дескриптора файла
    */
    if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
        err_quit("Невозможно получить максимальный номер дескриптора.");
    

    
    switch (fork())         //превращение в фоновый процесс 
    {
        case -1: err_sys("Ошибка вызова функции fork.");
        case 0:  break;                 //потомок проходит этот этап
        default: _exit(EXIT_SUCCESS);   //родитель завершается
    }
    
    if (setsid() == -1)                 //процесс становится лидером новой сессии
        syslog(LOG_ERR, "Не удалось стать лидером новой сессии %s: %s.", cmd, strerror(errno));

    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0) {
        syslog(LOG_ERR, "Невозможно игнорировать сигнадл SIGHUP %s: %s.", cmd, strerror(errno));
    }

    switch (fork())                     //делаем так, чтобы процесс
                                        //не стал лидером сессии 
    {
        case -1: {
            syslog(LOG_CRIT, "Ошибка вызова функции fork %s: %s.", cmd, strerror(errno));
            _exit(EXIT_FAILURE);
        }
        case 0:  break;                 
        default: _exit(EXIT_SUCCESS);   
    }
    
    umask(0);                    //сбрасываем маску режима создания файлов
    
    if (chdir("/") < 0)          //переходим в корневой каталог
        syslog(LOG_ERR, "Невозможно сделать текущим рабочим каталогом %s: %s.", cmd, strerror(errno));
                  

    
    if (rl.rlim_max == RLIM_INFINITY)
            rl.rlim_max = TD_MAX_CLOSE;
    for (rlim_t i = 0; i < rl.rlim_max; fd++)
        close(i);


    
    close(STDIN_FILENO);            //перенаправляем стандартные потоки
                                    //данных в /dev/null
    fd = open("/dev/null", O_RDWR);
    
    if (fd != STDIN_FILENO)
        syslog(LOG_ERR, "Ошибка открытия /dev/null %s: %s.", cmd, strerror(errno));

    if (dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO)
        syslog(LOG_ERR, "Ошибка дублирования потока STDOUT %s: %s.", cmd, strerror(errno));

    if (dup2(STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO)
        syslog(LOG_ERR, "Ошибка дублирования потока STDERR %s: %s.", cmd, strerror(errno));

    return 0;
}


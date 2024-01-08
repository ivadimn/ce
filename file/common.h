#ifndef _COMMON_H_
#define _COMMON_H_

#define _POSIX_C_SOURCE 200809L

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/termios.h>

#if !defined(TIOCGWINSZ)
#include <sys/ioctl.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define MAXLINE 4096

/*
* Права доступа по умолчанию к создаваемым файлам  
*/
#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
/*
* Права доступа по умолчанию к создаваемым каталогам  
*/
#define DIR_MODE (FILE_MODE | S_IXUSR | S_IXGRP | S_IXOTH)

typedef void Sigfunc(int);  //обработчик сигналов

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))


void err_msg(const char*, ...);
void err_dump(const char*, ...) __attribute__((noreturn));
void err_quit(const char*, ...) __attribute__((noreturn));
void err_cont(const char*, ...);
void err_exit(int, const char*, ...) __attribute__((noreturn));
void err_ret(const char*, ...);
void err_sys(const char*, ...) __attribute__((noreturn));

void log_msg(const char*, ...);
void log_open(const char*, int, int);
void log_quit(const char*, ...) __attribute__((noreturn));
void log_ret(const char*, ...);
void log_sys(const char*, ...) __attribute__((noreturn));
void log_exit(int, const char*, ...) __attribute__((noreturn));

#endif



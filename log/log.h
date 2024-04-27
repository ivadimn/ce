#ifndef _LOG_H_
#define _LOG_H_

#define  _XOPEN_SOURCE 700

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>


#define MAX_LEN 4006

void log_open(const char*);
void log_close();

void log_info(const char*, ...);
void log_msg(const char*, ...);

void log_quit(const char*, ...) __attribute__((noreturn));
void log_ret(const char*, ...);
void log_sys(const char*, ...) __attribute__((noreturn));
void log_exit(int, const char*, ...) __attribute__((noreturn));

#endif



#ifndef _DB_H_
#define _DB_H_

#include "params.h"
#include <sqlite3.h>

void open_db(void);
void close_db(void);
int get_count(void);
void str_param(char* data, char* param);

int select_all(param_t * param, int limit);

int insert(param_t * param);
int update(param_t * param);
int remove(const char* sql);

#endif
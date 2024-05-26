#ifndef _PSQL_H_
#define _PSQL_H_

#include <libpq-fe.h>

void open_psql(const char* host,
               const char* dbname,
               const char* user,
               const char* pwd);

void close_psql();
int select_psql_stat(const char* query, double* value);

#endif
#ifndef _LITE_H_
#define _LITE_H_

#include <sqlite3.h>


void open_sqlite(const char* dbname);
void close_sqlite();
int select_sqlite_stat(const char* query, double* value);

#endif
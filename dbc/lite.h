#ifndef _LITE_H_
#define _LITE_H_

#include <sqlite3.h>

typedef enum {CID, NAME, TYPE, NOTNULL, DIFLT_VALUE, PK} column_t;

sqlite3* open_sqlite(const char* dbname);
void close_sqlite(sqlite3* db);
int select_sqlite_stat(sqlite3* db, const char* query, double* value, char* err);
int is_valid_sqlite_param(sqlite3* db, const char* table, const char* column, char* err);

#endif
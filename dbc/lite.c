#include "lite.h"
#include "log.h"

static sqlite3 *db;

void open_sqlite(const char* dbname) {
    if (sqlite3_open(dbname, &db) != SQLITE_OK) {
        err_sys("Ошибка открытия БД: %s\n", sqlite3_errmsg(db));
    }
}

void close_sqlite() {
    sqlite3_close(db);
}

int select_sqlite_stat(const char* query, double* value) {
    int result;
    sqlite3_stmt* stmt; 

    if(sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        err_msg("Ошибка подготовки SQL-запроса: %s", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }

    result = sqlite3_step(stmt);
    
    if(result != SQLITE_ROW) {
        err_msg("Ошибка выполнения SQL-запроса: %s", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }
    *value = sqlite3_column_double(stmt, 0);
    sqlite3_finalize(stmt);
    return 0;
}
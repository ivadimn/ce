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

int is_valid_sqlite_column(const char* table, const char* column) {

    int not_null, pk, autoinc;
    const char *data_type = NULL;
    const char *collation = NULL;
    int result = sqlite3_table_column_metadata(db, NULL, table, column,
                                                &data_type, &collation, 
                                                &not_null, &pk, &autoinc);
    if(result == SQLITE_ERROR) {
        err_msg("Ошибка: %s", sqlite3_errmsg(db));    
    }

    printf("Type - %s, collation - %s\n", data_type, collation);
    return result;
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
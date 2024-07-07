#include "lite.h"
#include "log.h"
#include "db.h"

//static sqlite3 *db;

sqlite3* open_sqlite(const char *dbname)
{
    sqlite3 *db;
    if (sqlite3_open(dbname, &db) != SQLITE_OK)
    {
        sqlite3_close(db);
        err_sys("Ошибка открытия БД: %s\n", sqlite3_errmsg(db));
    }
    return db;
}

void close_sqlite(sqlite3 *db)
{
    sqlite3_close(db);
}

int is_valid_sqlite_param(sqlite3* db, const char *table, const char *column, char *err)
{
    int result;
    int column_founded = 0;
    char sql[SQL_LEN];
    sqlite3_stmt *stmt;
    const unsigned char *col_name;
    const unsigned char *col_type;

    snprintf(sql, SQL_LEN - 1, "PRAGMA table_info(%s); ", table);
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        snprintf(err, ERR_MSG_LEN - 1, "Ошибка подготовки SQL-запроса: %s", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }
    result = sqlite3_step(stmt);
    if (result == SQLITE_DONE)
    {
        snprintf(err, ERR_MSG_LEN - 1, "Таблица с именем %s не найдена", table);
        sqlite3_finalize(stmt);
        return -1;
    }

    while (result == SQLITE_ROW)
    {
        col_name = sqlite3_column_text(stmt, NAME);
        col_type = sqlite3_column_text(stmt, TYPE);
        if (strcmp((char*)col_name, column) == 0) 
        {
            column_founded = 1;
            if (strcmp((char*)col_type, "TEXT") == 0 || strcmp((char*)col_type, "BLOB") == 0)
            {
                snprintf(err, ERR_MSG_LEN - 1, "Kолонка %s имеет не числовой тип данных: %s", column, col_type);
                sqlite3_finalize(stmt);
                return -1;
            }
            break;
        }
        result = sqlite3_step(stmt);
    }
    
    if (!column_founded)
    {
        snprintf(err, ERR_MSG_LEN - 1, "Колонка с именем %s не найдена", column);
        sqlite3_finalize(stmt);
        return -1;
    }
    
    sqlite3_finalize(stmt);
    return 0;    
}

int select_sqlite_stat(sqlite3* db, const char *query, double *value, char *err)
{
    int result;
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK)
    {
        snprintf(err, ERR_MSG_LEN - 1, "Ошибка подготовки SQL-запроса: %s", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }

    result = sqlite3_step(stmt);

    if (result != SQLITE_ROW)
    {
        snprintf(err, ERR_MSG_LEN - 1, "Ошибка выполнения SQL-запроса: %s", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }
    *value = sqlite3_column_double(stmt, 0);
    sqlite3_finalize(stmt);
    return 0;
}
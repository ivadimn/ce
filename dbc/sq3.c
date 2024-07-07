#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#define SQL_LEN 4096

typedef enum {CID, NAME, TYPE, NOTNULL, DIFLT_VALUE, PK} column_t;

static sqlite3 *db;

void open_sqlite(const char *dbname)
{
    if (sqlite3_open(dbname, &db) != SQLITE_OK)
    {
        sqlite3_close(db);
        printf("Ошибка открытия БД: %s\n", sqlite3_errmsg(db));
        exit(EXIT_FAILURE);
    }
}

void close_sqlite()
{
    sqlite3_close(db);
}

int is_valid_sqlite_param(const char *table, const char *column, char *err)
{
    int result;
    int column_founded = 0;
    char sql[SQL_LEN];
    sqlite3_stmt *stmt;
    const char *col_name;
    const char *col_type;

    snprintf(sql, SQL_LEN - 1, "PRAGMA table_info(%s); ", table);
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        snprintf(err, 255, "Ошибка подготовки SQL-запроса: %s", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }
    result = sqlite3_step(stmt);
    if (result == SQLITE_DONE)
    {
        snprintf(err, 255, "Таблица с именем %s не найдена", table);
        sqlite3_finalize(stmt);
        return -1;
    }

    while (result == SQLITE_ROW)
    {
        col_name = sqlite3_column_text(stmt, NAME);
        col_type = sqlite3_column_text(stmt, TYPE);
        if (strcmp(col_name, column) == 0) 
        {
            column_founded = 1;
            if (strcmp(col_type, "TEXT") == 0 || strcmp(col_type, "BLOB") == 0)
            {
                snprintf(err, 255, "Kолонка %s имеет не числовой тип данных: %s", column, col_type);
                sqlite3_finalize(stmt);
                return -1;
            }
            break;
        }
        result = sqlite3_step(stmt);
    }
    
    /*if (result != SQLITE_DONE)
    {
        snprintf(err, 255, "Ошибка выполнения SQL-запроса: %s", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }*/

    if (!column_founded)
    {
        snprintf(err, 255, "Колонка с именем %s не найдена", column);
        sqlite3_finalize(stmt);
        return -1;
    }

    sqlite3_finalize(stmt);
    return 0;
}

int main(int argc, char** argv) {
       
    char error_message[1024];
    open_sqlite(argv[1]);
    if (is_valid_sqlite_param(argv[2], argv[3], error_message) == 0)
        printf("Всё хорошо\n");
    else    
        printf("Ошибка: %s\n", error_message);
    close_sqlite();
    return 0;
}




#include "db.h"
#include "log.h"

static  const char* db_name = "fsize_conf.db";
static sqlite3 *db;
static char sql[SQL_LEN];


const char* sql_select_all = "SELECT id, name, value, ex_value, description FROM param; ";
const char* sql_select_one = "SELECT id, name, value, ex_value, description FROM param \
WHERE name = %s; ";
const char* sql_select_count = "SELECT COUNT(*) FROM param ;";
const char* sql_insert = "INSERT INTO param(name, value, ex_value, description) \
values(%s, %s, %s, %s); ";
const char* sql_update = "UPDATE param SET value=%s, ex_value=%s, description=%s \
WHERE name=%s; ";



void open_db(void) {
    if (sqlite3_open(db_name, &db) != SQLITE_OK) {
        err_sys("Ошибка открытия БД: %s\n", sqlite3_errmsg(db));
    }
}

void close_db(void) {

    sqlite3_close(db);
}

void str_param(char* data, char* param) {
    size_t len = strlen(data);
    if (len == 0) {
        strcpy(param, "NULL");
        return;
    }
    param[0] = '\'';
    for (size_t i = 0; i < len; i++)
        param[i + 1] = data[i];
    param[len + 1] = '\'';
    param[len + 2] = '\0';
}

int get_count(void) {
    sqlite3_stmt* stmt; 
    char *err = NULL;
    int rc = 0;
    int count = 0;

    if(sqlite3_prepare_v2(db, sql_select_count, -1, &stmt, NULL) != SQLITE_OK) {
        err_msg("Ошибка подготовки SQL-запроса: %s", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }
    while((rc = sqlite3_step(stmt)) == SQLITE_ROW)   {
        count = sqlite3_column_int(stmt, 0);
    }
    if(rc != SQLITE_DONE) {
        err_msg("Ошибка выполнения SQL-запроса: %s", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }
    sqlite3_finalize(stmt);
    return count;
}

int select_all(param_t* param, int limit) {
    sqlite3_stmt* stmt; 
    int rc = 0;
    int index = 0;
    const char* tmp;

    if(sqlite3_prepare_v2(db, sql_select_all, -1, &stmt, NULL) != SQLITE_OK) {
        err_msg("Ошибка подготовки SQL-запроса: %s", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }

    while((rc = sqlite3_step(stmt)) == SQLITE_ROW && index < limit)   {
        param[index].id = sqlite3_column_int(stmt, 0);
        strcpy(param[index].name, sqlite3_column_text(stmt, 1));
        strcpy(param[index].value, sqlite3_column_text(stmt, 2));
        tmp = sqlite3_column_text(stmt, 3);
        if (tmp)
            strcpy(param[index].ex_value, tmp);
        tmp = sqlite3_column_text(stmt, 4);
        if (tmp)
            strcpy(param[index].description, tmp);
        index++;
    }

    if(rc != SQLITE_DONE) {
        err_msg("Ошибка выполнения SQL-запроса: %s", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }

    sqlite3_finalize(stmt);
    return index + 1;
} 


int insert(param_t *param) {

    char *err = NULL;
    char p[4][MAX_LEN];
    str_param(param->name, p[0]);
    str_param(param->value, p[1]);
    str_param(param->ex_value, p[2]);
    str_param(param->description, p[3]);

    sprintf(sql, sql_insert, p[0], p[1], p[2], p[3]);
    
    if (sqlite3_exec(db, sql, NULL, NULL, &err)) {
        err_msg("Ошибка вставки записи: %s", err);
        sqlite3_free(err);
        return -1;
    }
    return 1;
}

int update(param_t * param) {
    char *err = NULL;
    char p[4][MAX_LEN];
    str_param(param->name, p[0]);
    str_param(param->value, p[1]);
    str_param(param->ex_value, p[2]);
    str_param(param->description, p[3]);

    sprintf(sql, sql_update, p[1], p[2], p[3], p[0]);

    if (sqlite3_exec(db, sql, NULL, NULL, &err)) {
        err_msg("Ошибка изменения записи: %s", err);
        sqlite3_free(err);
        return -1;
    }
    return 1;
}

int remove(const char* sql) {

}
#include "db.h"
#include "log.h"
#include "lite.h"
#include "psql.h"

static engine_t db_engine;
static char sql[SQL_LEN];
static char error_message[ERR_MSG_LEN];

static char *sql_stat = "SELECT %s(%s) FROM %s; ";
static char *sql_disp = "SELECT AVG((%s - %f) * (%s - %f)) FROM %s; ";

void* open_db(conninfo_t *conninfo)
{

    if (strcmp(conninfo->dbms, "SQLITE") == 0)
    {
        db_engine = SQLITE;
        return open_sqlite(conninfo->dbname);
    }
    else if (strcmp(conninfo->dbms, "POSTGRESQL") == 0)
    {
        db_engine = POSTGRESQL;
        return open_psql(conninfo->hostaddr, conninfo->dbname,
                  conninfo->user, conninfo->password);
    }
    else
    {
        err_quit("Неизвестный тип СУБД '%s'", conninfo->dbms);
    }
}

void close_db(void* db)
{
    switch (db_engine)
    {
        case SQLITE:
            close_sqlite((sqlite3*) db);
            break;
        case POSTGRESQL:
            close_psql((PGconn*) db);
            break;
        default:
            break;
    }
}

char *get_err_msg()
{
    return error_message;
}

int is_valid_param(void* db, const char *table, const char *column)
{
    switch (db_engine)
    {
        case SQLITE:
            return is_valid_sqlite_param((sqlite3*)db, table, column, error_message);
        case POSTGRESQL:
            return is_valid_psql_param((PGconn*)db, table, column, error_message);
        default:
            return INVALID_PARAM;
    }
}

static int select_stat(void* db, const char *fun_name, const char *table, const char *column, double *result)
{

    double r_avg;
    if (strcmp(fun_name, "DISP") == 0)
    {
        avg(db, table, column, &r_avg);
        snprintf(sql, SQL_LEN - 1, sql_disp, column, r_avg, column, r_avg, table);
    }
    else
    {
        snprintf(sql, SQL_LEN - 1, sql_stat, fun_name, column, table);
    }

    switch (db_engine)
    {
        case SQLITE:
            return select_sqlite_stat((sqlite3*)db, sql, result, error_message);
        case POSTGRESQL:
            return select_psql_stat((PGconn*)db, sql, result, error_message);
        default:
            return -1;
    }
}

int avg(void* db, const char *table, const char *column, double *result)
{
    return select_stat(db, "AVG", table, column, result);
}

int min(void* db, const char *table, const char *column, double *result)
{
    return select_stat(db, "MIN", table, column, result);
}

int max(void* db, const char *table, const char *column, double *result)
{
    return select_stat(db, "MAX", table, column, result);
}

int sum(void* db, const char *table, const char *column, double *result)
{
    return select_stat(db, "SUM", table, column, result);
}

int disp(void* db, const char *table, const char *column, double *result)
{
    return select_stat(db, "DISP", table, column, result);
}

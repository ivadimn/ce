#include "log.h"
#include "psql.h"
#include "db.h"

//static PGconn *db;
static char *select_table = "SELECT table_name \
            FROM information_schema.tables \
            WHERE table_name = '%s' ; ";

static char *select_column = "SELECT numeric_precision \
                             FROM information_schema.columns \
                             WHERE table_name = '%s' and column_name = '%s'; ";

PGconn *open_psql(const char *host,
               const char *dbname,
               const char *user,
               const char *pwd)
{
    PGconn *db;
    char conninfo[MAX_LEN];
    snprintf(conninfo, MAX_LEN - 1, "hostaddr=%s port=5432 user=%s password=%s dbname=%s",
             host, user, pwd, dbname);
    db = PQconnectdb(conninfo);
    if (PQstatus(db) != CONNECTION_OK)
    {
        err_msg("Ошибка подключения к серверу: %s", PQerrorMessage(db));
        PQfinish(db);
        exit(EXIT_FAILURE);
    }
    return db;
}

void close_psql(PGconn *db)
{
    PQfinish(db);
}

static int is_psql_table(PGconn *db, const char *table, char *err)
{
    PGresult *result = NULL;
    char sql[QUERY_LEN];
    snprintf(sql, QUERY_LEN - 1, select_table, table);
    result = PQexec(db, sql);
    if (PQresultStatus(result) != PGRES_TUPLES_OK)
    {
        snprintf(err, ERR_MSG_LEN - 1, "Ошибка при получении информации о таблице '%s': %s", table, PQresultErrorMessage(result));
        PQclear(result);
        return -1;
    }
    int rows_count = PQntuples(result);
    if (rows_count <= 0)
    {
        snprintf(err, ERR_MSG_LEN - 1, "Таблица '%s' не найдена", table);
        PQclear(result);
        return -1;
    }
    PQclear(result);
    return 0;
}

static int is_psql_column(PGconn *db, const char *table, const char *column, char *err)
{
    PGresult *result = NULL;
    char sql[QUERY_LEN];
    snprintf(sql, QUERY_LEN - 1, select_column, table, column);
    result = PQexec(db, sql);
    if (PQresultStatus(result) != PGRES_TUPLES_OK)
    {
        snprintf(err, ERR_MSG_LEN - 1, "Ошибка при получении информации о колонке '%s': %s", column, PQresultErrorMessage(result));
        PQclear(result);
        return -1;
    }
    int rows_count = PQntuples(result);
    if (rows_count <= 0)
    {
        snprintf(err, ERR_MSG_LEN - 1, "Колонка '%s' не найдена", column);
        PQclear(result);
        return -1;
    }
    if (PQgetisnull(result, 0, 0))
    {
        snprintf(err, ERR_MSG_LEN - 1, "Колонка '%s' не может использоваться в математических операциях", column);
        PQclear(result);
        return -1;
    }
    PQclear(result);
    return 0;
}

int is_valid_psql_param(PGconn *db, const char *table, const char *column, char *err)
{
    if (is_psql_table(db, table, err) < 0)
    {
        return -1;
    }
    if (is_psql_column(db, table, column, err) < 0)
    {
        return -1;
    }
    return 0;
}

int select_psql_stat(PGconn *db, const char *query, double *value, char *err)
{

    PGresult *result;
    result = PQexec(db, query);
    if (PQresultStatus(result) != PGRES_TUPLES_OK)
    {
        snprintf(err, ERR_MSG_LEN - 1, "Ошибка выполнения SQL-запроса: %s", PQerrorMessage(db));
        PQclear(result);
        return -1;
    }

    *value = strtod(PQgetvalue(result, 0, 0), NULL);
    PQclear(result);
    return 0;
}
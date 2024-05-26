#include "log.h"
#include "psql.h"

static PGconn* db;

void open_psql(const char* host,
               const char* dbname,
               const char* user,
               const char* pwd) {

    char conninfo[MAX_LEN];
    sprintf(conninfo, "hostaddr=%s port=5432 user=%s password=%s dbname=%s", 
                host, user, pwd, dbname);
    db = PQconnectdb(conninfo);
    if (PQstatus(db) != CONNECTION_OK) {
        err_msg("Ошибка подключения к серверу: %s", PQerrorMessage(db));
        PQfinish(db);
        exit(EXIT_FAILURE);
    }
}

void close_psql() {
     PQfinish(db);
}

int select_psql_stat(const char* query, double* value) {

    PGresult* result;
    printf("%s\n", query);
    result = PQexec(db, query);
    if (PQresultStatus(result) != PGRES_TUPLES_OK) {
        err_msg("Ошибка выполнения SQL-запроса: %s", PQerrorMessage(db));
        PQclear(result);
        return -1;
    }
    printf("%s\n", PQgetvalue(result, 0, 0));  
    *value = strtod(PQgetvalue(result, 0, 0), NULL);
    PQclear(result);
    return 0;
}
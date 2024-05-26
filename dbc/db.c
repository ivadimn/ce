#include "db.h"
#include "log.h"
#include "lite.h"
#include "psql.h"


static engine_t db_engine;
static char sql[SQL_LEN];


const char* select_stat = "SELECT %s(%s) FROM %s; ";


void open_db(engine_t engine, conninfo_t* conninfo) {

    switch (engine)  {
        case SQLITE:
            db_engine = SQLITE;
            open_sqlite(conninfo->dbname);
            break;
        case POSTGRESQL:
            db_engine = POSTGRESQL;
            open_psql(conninfo->hostaddr, conninfo->dbname, 
                      conninfo->user, conninfo->password);

            break;
        default:
            break;
    }
    
}

void close_db(void) {
    switch (db_engine)  {
        case SQLITE:
            close_sqlite();
            break;
        case POSTGRESQL:
            close_psql();
            break;
        default:
            break;
    }
}

int avg(const char* table, const char* column, double* result) {
    
    sprintf(sql, select_stat, "AVG", column, table);

    switch (db_engine)
    {
    case SQLITE:
        return select_sqlite_stat(sql, result);
    case POSTGRESQL:
        return select_psql_stat(sql, result);
    default:
        break;
    }
    
    return -1;
}


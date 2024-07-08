#include "db.h"

typedef struct {
    sqlite3* db;
    char dbname[DBNAME_LEN];
} connection_t;


connection_t* connect(const char* dbname) {
    
}


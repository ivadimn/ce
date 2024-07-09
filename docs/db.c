#include "def.h"
#include "db.h"
#include "log.h"

typedef struct {
    sqlite3* db;
    char dbname[DBNAME_LEN];
} connection_t;

static char* error_message[ERR_MSG_LEN];

char* get_error_message() {
    return error_message;
}

connection_t* connect(const char* dbname) {
    
}


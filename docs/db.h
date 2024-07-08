#ifndef _DB_H_
#define _DB_H_

#define DBNAME_LEN 64

#include <sqlite3.h>

typedef struct connection_t;

connection_t* connect(const char* dbname);


#endif
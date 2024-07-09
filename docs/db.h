#ifndef _DB_H_
#define _DB_H_

#include <sqlite3.h>

typedef struct connection_t;

char* get_error_message();

connection_t* connect(const char* dbname);


#endif
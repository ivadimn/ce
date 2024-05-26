#ifndef _DB_H_
#define _DB_H_


#define SQL_LEN 4096
#define NAME_LEN 64
#define INFO_LEN 23
#define IP4_LEN 16

typedef enum { SQLITE, POSTGRESQL } engine_t;

typedef struct {
    char host[NAME_LEN];
    char hostaddr[IP4_LEN];
    char dbname[INFO_LEN];
    char user[INFO_LEN];
    char password[INFO_LEN];
} conninfo_t;

void open_db(engine_t engine, conninfo_t* conninfo);
void close_db(void);

int avg(const char* table, const char* column, double* result);


#endif
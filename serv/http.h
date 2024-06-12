#ifndef _HTTP_H_
#define _HTTP_H_

#include <sys/epoll.h>

#define MAX_URI 2048
#define VER_LEN 16
#define METHOD_LEN 16
#define STATUS_LEN 64
#define HEADER_NAME_LEN 32
#define HEADER_VALUE_LEN 255

typedef enum {OPTIONS, GET, HEAD, POST, PUT, PATCH, DELETE, TRACE, CONNECT } method_t;
typedef enum {ACCEPTED, READED, WROTE } se_state_t;

typedef struct {
    char name[HEADER_NAME_LEN];
    char value[HEADER_VALUE_LEN];
} header_t;

typedef struct {
    char method[METHOD_LEN];
    char uri[MAX_URI];
    char version[VER_LEN];
    int hcount;
    header_t* headers;
} request_t;

typedef struct {
    char status[STATUS_LEN];
    int hcount;
    header_t* headers;
    unsigned char* body;
} response_t;

typedef struct {
    se_state_t state;
    request_t reqv;
    int fd;
} session_t;

void print_request(session_t* session);
int set_non_blocking(int sock);
int init_session(int fd, struct epoll_event* ev);

void get_request(char* buffer, request_t* request);

void prepare_response(response_t* response, int status_code);

void read_socket(session_t* session);
void write_socket(session_t* session);

#endif
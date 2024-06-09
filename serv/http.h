#ifndef _HTTP_H_
#define _HTTP_H_

#define MAX_URI 2048
#define VER_LEN 16
#define METHOD_LEN 16
#define STATUS_LEN 64
#define HEADER_NAME_LEN 32
#define HEADER_VALUE_LEN 255

typedef enum {OPTIONS, GET, HEAD, POST, PUT, PATCH, DELETE, TRACE, CONNECT } method_t;

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


void get_request(char* buffer, request_t* request);



#endif
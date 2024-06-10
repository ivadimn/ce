#include "http.h"
#include "vstr.h"
#include <string.h>
#include <time.h>
#include <stdio.h>

static char *methods[] = {
    "OPTIONS", "GET", "HEAD", "POST", "PUT", "PATCH", "DELETE", "TRACE", "CONNECT" };

static void _get_start_line(vstr_t* sl, request_t* request) {
    vstr_array_t* sl_arr = vstr_array_create(3);
    vstr_split(sl_arr, sl, " ", NULL, NULL);
    strcpy(request->method, (char*)sl_arr->array[0]->data);
    strcpy(request->uri, (char*)sl_arr->array[1]->data);
    strcpy(request->version, (char*)sl_arr->array[0]->data);
    vstr_array_free(sl_arr);
}

static void _get_headers(vstr_array_t* list, request_t* request) {
    long index, pos;
    vstr_t* header;
    request->hcount = list->length - 1;
    request->headers = (header_t*) malloc(sizeof(header_t) * request->hcount);
    for (int i = 0; i < list->length - 1; i++) {
        header = vstr_array_get(list, i + 1);
        index = vstr_in(header, ':');
        pos = index + 2; 
        strncpy(request->headers[i].name, (char*)header->data, index);
        strncpy(request->headers[i].value, (char*)(header->data + pos), header->length - pos - 1);
    }
}

void get_request(char* buffer, request_t* request) {
    
    vstr_array_t* list = vstr_array_create(16);
    vstr_t* req = vstr_dup(buffer);
    vstr_split(list, req, "\r\n", NULL, NULL);    
    _get_start_line(vstr_array_get(list, 0), request);
    _get_headers(list, request);
    vstr_array_free(list);
    vstr_free(req);
}

void init_session(int fd) {
    
}

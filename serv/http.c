#include "log.h"
#include "http.h"
#include "vstr.h"
#include "utils.h"
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>

static char buffer[2048];

static char *methods[] = {
    "OPTIONS", "GET", "HEAD", "POST", "PUT", "PATCH", "DELETE", "TRACE", "CONNECT" };

static char *response_headers[] = {
    "Date",
    "Server",
    "Content-Length",
    "Content-Type"
};    


static void _get_start_line(vstr_t* sl, request_t* request) {
    vstr_array_t* sl_arr = vstr_array_create(3);
    vstr_split(sl_arr, sl, " ", NULL, NULL);
    strcpy(request->method, (char*)sl_arr->array[0]->data);
    vstr_cut(sl_arr->array[1], 1, 0);
    strcpy(request->uri, (char*)sl_arr->array[1]->data);
    strcpy(request->version, (char*)sl_arr->array[2]->data);
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
        request->headers[i].name[index] = '\0';
        strncpy(request->headers[i].value, (char*)(header->data + pos), header->length - pos);
        request->headers[i].value[header->length - pos] = '\0';
    }
}


void print_request(session_t* session) {
    request_t reqv = session->reqv;
    printf("Метод: %s\n", reqv.method);
    printf("URI: %s\n", reqv.uri);
    printf("Версия: %s\n", reqv.version);
    for (long i = 0; i < reqv.hcount; i++) {
        printf("%s: %s\n", reqv.headers[i].name, reqv.headers[i].value);
    }
}

/*
* установить неблокирующий режим для сокета
*/
int set_non_blocking(int sock) {
	 int opts;

 	opts = fcntl(sock,F_GETFL);
 	if (opts < 0) {
  		perror("fcntl(F_GETFL)");
  		return -1;
 	}
 	opts = (opts | O_NONBLOCK);
 	if (fcntl(sock,F_SETFL,opts) < 0) {
  		perror("fcntl(F_SETFL)");
  		return -1;
 	}
	return 0;
}

/*
* Получение параметров запроса
*/
void get_request(char* buffer, request_t* request) {
    
    vstr_array_t* list = vstr_array_create(16);
    vstr_t* req = vstr_dup(buffer);
    vstr_split(list, req, "\r\n", NULL, NULL);   
    _get_start_line(vstr_array_get(list, 0), request);
    _get_headers(list, request);
    vstr_array_free(list);
    vstr_free(req);
}


void prepare_response(response_t* response, int status_code) {
    char time_buf[HEADER_VALUE_LEN];
    time_t mytime = time(NULL);
    struct tm *now = localtime(&mytime);
    strftime(time, MAX_LEN - 1, "%a, %d %b %Y %H:%M:%S %Z", now);
    snprintf(response->status, STATUS_LEN, "%s %d %s\n\r", "HTTP\1.1", 200, "OK");
    response->hcount = 4;
    response->headers = (header_t*) alloc(sizeof(header_t) * response->hcount);
    strcpy(response->headers[0].name, "Date");
    
    for (size_t i = 0; i < response->hcount; i++) {
        
    }
    

}


int init_session(int fd, struct epoll_event *ev) {
    session_t* session = (session_t*) malloc(sizeof(session_t));
    if (session == NULL) {
        err_msg("Ошибка распределения памяти.");
        return -1;
    }
    session->fd = fd;
    session->state = ACCEPTED;
    ev->data.ptr = session;
    return 0;     
}

void read_socket(session_t* session) {
	int rc = recv(session->fd, buffer, sizeof(buffer), 0);
 	if (rc < 0)  {
  		err_ret("Ошибка чтения сокета ...");
  		return;
 	}
 	buffer[rc] = 0;
	get_request(buffer, &session->reqv);
    print_request(session);

}

void write_socket(session_t* session) {

}

#include "log.h"
#include "http.h"
#include "vstr.h"
#include "utils.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>

char *body = "<!DOCTYPE html> \
<html> \
<head> \
    <meta charset=\"UTF-8\"> \
    <title>Магазин бытовой техники Comfort</title> \
</head> \
<body> \
Hello from server!!! \
</body> \
</html>";

static char buffer[2048];
static char work_dir[1024];

static char *methods[] = {
    "OPTIONS", "GET", "HEAD", "POST", "PUT", "PATCH", "DELETE", "TRACE", "CONNECT" };

static char *response_headers[] = {
    "Date",
    "Server",
    "Content-Length",
    "Content-Type"
};    

static int get_status_code() {
    switch (errno) {
        case EBADF:
        case ENOENT:
            return 404;
        case EACCES:
        case EMFILE:
            return 403;
        default:
            return 404;
    }
}

static char* get_status_message(int status_code) {
    switch (status_code) {
        case 200:
            return "OK";
        case 404:
            return "Not Found";
        case 403:
            return "Forbidden";
        default:
            return "Not Found";
    }
}


void set_work_dir(const char* dir) {
    long len = strlen(dir);
    if(len >= 1023)
        err_quit("Слишком длинное имя каталога.");
    strcpy(work_dir, dir);
    if (work_dir[len - 1] != '/') {
        work_dir[len] = '/';
        work_dir[len + 1] = '\0';
    }
}


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

int init_session(int fd, struct epoll_event *ev) {
    session_t* session = (session_t*) malloc(sizeof(session_t));
    if (session == NULL) {
        err_msg("Ошибка распределения памяти.");
        return -1;
    }
    session->fd = fd;
    session->state = ACCEPTED;
    session->is_ready = 0;
    ev->data.ptr = session;
    return 0;     
}

/*
* закрыть сессию с клиетом
*/
int close_session(session_t* session) {
    return 0;
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


void prepare_response(session_t* session) {

    int status_code = 200;
    char filename[MAX_PATH];
    time_t mytime = time(NULL);
    struct tm *now = localtime(&mytime);
    
    snprintf(filename, MAX_PATH - 1, "%s%s", work_dir, session->reqv.uri);
    size_t fsize = check_file(filename);
    //size_t fsize = strlen(body);
    if(fsize == 0) {
        status_code = get_status_code();
    }
    session->resp.fsize = fsize;
    snprintf(session->resp.status, STATUS_LEN, "%s %d %s\n\r", "HTTP/1.1", status_code, get_status_message(status_code));
    session->resp.hcount = 5;
    session->resp.headers = (header_t*) alloc(sizeof(header_t) * session->resp.hcount);
    strcpy(session->resp.headers[0].name, "Date");
    strftime(session->resp.headers[0].value, HEADER_VALUE_LEN - 1, "%a, %d %b %Y %H:%M:%S %Z", now);
    strcpy(session->resp.headers[1].name, "Server");
    strcpy(session->resp.headers[1].value, "ivadimn.ru");
    strcpy(session->resp.headers[2].name, "Content-Length");
    snprintf(session->resp.headers[2].value, HEADER_VALUE_LEN - 1, "%ld", fsize);
    strcpy(session->resp.headers[3].name, "Connection");
    strcpy(session->resp.headers[3].value, "close");
    strcpy(session->resp.headers[4].name, "Content-Type");
    strcpy(session->resp.headers[4].value, "text/html;charset=utf-8");
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
    prepare_response(session);

}

void write_socket(session_t* session) {

    int rc = 0, send_count = 0;
    uint8_t *data;
    long send_len;
    char filename[MAX_PATH];
    snprintf(filename, MAX_PATH - 1, "%s%s", work_dir, session->reqv.uri);

    vstr_t *buffer = vstr_create((HEADER_NAME_LEN + HEADER_VALUE_LEN) * 
                        (session->resp.hcount + 1) + session->resp.fsize);
    buffer = vstr_append(buffer, session->resp.status);
    for (int i = 0; i < session->resp.hcount; i++) {
        buffer = vstr_append(buffer, session->resp.headers[i].name);
        buffer = vstr_append(buffer, ": ");
        buffer = vstr_append(buffer, session->resp.headers[i].value);
        buffer = vstr_append(buffer, "\r\n");
    }
    buffer = vstr_append(buffer, "\r\n");
    printf("Len headers %ld\n", buffer->length);
    printf("file size %ld\n", session->resp.fsize);
     
    //read_file(filename, session->resp.fsize, (char*) buffer->data, buffer->length);
    //buffer->length += session->resp.fsize;
    buffer = vstr_append(buffer, body);
    printf("Total Len %ld\n", buffer->length);
    send_len = buffer->length;
    data = buffer->data;
    vstr_print(buffer, stdout);
    do {
        printf("Before send\n");
        rc = send(session->fd, data, send_len, 0);
        printf("rc - %d\n", rc);
        if (rc < 0) {
            err_ret("Ошибка записи в сокет");
            return;
        }
        send_count += rc;
        data += send_count;
        send_len -= send_count;
        printf("send count - %d\n", send_count);
    } while(send_count < buffer->length);
    
    vstr_free(buffer);
}

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <unistd.h>

#include "log.h"
#include "http.h"


static char buffer[2048];

static struct option options[] = {
		{"help", no_argument, 0, 'h'},
		{"port", required_argument, 0, 'p'},
    	/*{"dbname", required_argument, 0, 'd'},
		{"table", required_argument, 0, 't'},
		{"column", required_argument, 0, 'c'},*/
		{NULL, 0, 0, 0}
};

/*
 *  печатает help по использованию приложения
 */
void print_help(char* app_name)
{
	printf("\n Использование: %s port\n\n", app_name);
	printf("  Options:\n");
	printf("   -h --help                 		Печатает эту справку\n");
	printf("   -p --port=PORT_NUMBER		 	Номер порта\n");
	/*printf("   -d --dbname=DATABASE_NAME 		Наименование базы данных\n");
	printf("   -t --table=TABLE_NAME     		Наименование таблицы\n");
  	printf("   -с --column=COLUMN_NAME   		Наименование колонки\n");*/
	printf("\n");
}

int get_line(char* line, int size) {
    char ch;
    int index = 0;
    while ((ch = getchar()) != '\n' && index < size - 1) {
        line[index++] = ch;
    }
    line[index] = '\0';    
    return index - 1;
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
* прочитать данные из сокета
*/
void do_read(int fd) {
	request_t req;
	int rc = recv(fd, buffer, sizeof(buffer), 0);
 	if (rc < 0)  {
  		err_ret("Ошибка чтения сокета ...");
  		return;
 	}
 	buffer[rc] = 0;
	get_request(buffer, &req);
 	
}

/*
* записать данные в сокет
*/
void do_write(int fd) {
 	int rc = send(fd, buffer, strlen(buffer), 0);
 	if (rc < 0) {
  		err_ret("Ошибка записи в сокет");
  		return;
 	}
}

#define BACKLOG 512
#define MAX_EVENTS 128
#define MAX_MESSAGE_LEN 2048



int main (int argc,char **argv)
{
	int value, option_index = 0;
	int port;


	if (argc == 1) 	{
		print_help(argv[0]);
		return EXIT_FAILURE;
	}
	
	while ((value = getopt_long(argc, argv, "hp:", options, &option_index)) != -1) {
        char* p;
		switch (value) {
			case 'h':
				print_help(argv[0]);
				return EXIT_FAILURE;
			case 'p':
				
				port = strtol(optarg, &p, 10);
				break;
			case '?':
				print_help(argv[0]);
				return EXIT_FAILURE;
			default:
				break;
		}
	}

    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    char buffer[MAX_MESSAGE_LEN];
    memset(buffer, 0, sizeof(buffer));
    
	signal(SIGPIPE, SIG_IGN);  //игнорируем сигнал SIGPIPE

	/*
	* создаём серверный сокет и делаем его неблокирующим
	*/
	int sock_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
 	if (sock_listen_fd < 0)  {
  		err_sys("Ошибка сосздания сокета.");
 	}
 	set_non_blocking(sock_listen_fd);
	/*
	* привязываем сокет к адресу и порту и начинаем слушать
	*/
	memset((char*)&server_addr, 0, sizeof(server_addr));
 	server_addr.sin_family = AF_INET;
 	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
 	server_addr.sin_port = htons(port);

 	if (bind(sock_listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
  		err_sys("Ошибка привязки сокета.");
  	}

 	if (listen(sock_listen_fd, BACKLOG) < 0)  {
  		err_sys("Ошибка начала просдушивания сокета.");
 	}

    printf("Epoll echo server listening for connections on port: %d\n", port);
	/*
	* добавляем главный серверный сокет с список слежения epoll
	*/
	struct epoll_event ev, events[MAX_EVENTS]; //структра для сетевых соединений
    int new_events, sock_conn_fd, epollfd;

    epollfd = epoll_create(MAX_EVENTS);
    if (epollfd < 0) {
        err_sys("Ошибка создания epoll...");
    }
    

 	ev.events = EPOLLIN | EPOLLET;
 	ev.data.fd = sock_listen_fd;
 	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sock_listen_fd, &ev) < 0) {
  		err_sys("Ошибка вызова epoll_ctl");
 	}
 	int events_count = 1;
	//запускаем главный серверный цикл
	for(;;)	{
		//ждём сетевых соединений
		new_events = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (new_events == -1) {
            err_sys("Ошибка в epoll_wait ...");
        }
        

		for (int i = 0; i < new_events; i++) {
			//обрабатываем серверный дескриптор
			if (events[i].data.fd == sock_listen_fd) {
    			sock_conn_fd = accept(sock_listen_fd, (struct sockaddr*) &client_addr, &client_len);
    			if (sock_conn_fd < 0) {      //соединились но что-то не так
     				err_ret("Ошибка нового соединения ...");
     				continue;
    			}
                if (events_count == MAX_EVENTS - 1) {
     				err_msg("Массив событий заполнен\n");
     				close(sock_conn_fd);
     				continue;
    			}

				//приняли соединение, делаем его не блокирующим
				//и добавляем в пул epoll
                set_non_blocking(sock_conn_fd);
    			ev.data.fd = sock_conn_fd;
    			ev.events = EPOLLIN | EPOLLOUT | EPOLLET | EPOLLRDHUP;
    			if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sock_conn_fd, &ev) < 0) {
     				err_ret("Ошибка в epoll_ctl ...");
     				close(sock_conn_fd);
     				continue;
    			}
		    	events_count++;

   			} else {    //не сереверный дескриптор
				int fd = events[i].data.fd;

			    if (events[i].events & EPOLLIN)    //читаем если что-то пришло
     				do_read(fd);

    			if (events[i].events & EPOLLOUT)	//или пишем если надо что-то заисать
     				do_write(fd);

    			if (events[i].events & EPOLLRDHUP)
     				err_msg("Ошибка на сокете: %d", fd);

				//и рвём соединение
    			//epoll_ctl(, EPOLL_CTL_DEL, fd, &connev);
    			//close(fd);
    			//events_count--;
			}
		}
	}

	return 0;
}



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
	int rc = recv(fd, buffer, sizeof(buffer), 0);
 	if (rc < 0)  {
  		perror("read");
  		return;
 	}
 	buffer[rc] = 0;
 	printf("read: %s\n", buffer);
}

/*
* записать данные в сокет
*/
void do_write(int fd) {
	static const char* greeting = "O hai!\n";
 	int rc = send(fd, greeting, strlen(greeting), 0);
 	if (rc < 0) {
  		perror("write");
  		return;
 	}
}


void process_error(int fd)  {
	printf("fd %d error!\n", fd);
}

#define MAX_EPOLL_EVENTS 128
static struct epoll_event events[MAX_EPOLL_EVENTS];

#define BACKLOG 128

int main (int argc,char **argv)
{
	int value, option_index = 0;
	int port;

	if (argc == 1) 	{
		print_help(argv[0]);
		return EXIT_FAILURE;
	}
	
	while ((value = getopt_long(argc, argv, "hp:", options, &option_index)) != -1) {
		switch (value) {
			case 'h':
				print_help(argv[0]);
				return EXIT_FAILURE;
			case 'p':
				char* p;
				port = strtol(optarg, &p, 10);
				break;
			case '?':
				print_help(argv[0]);
				return EXIT_FAILURE;
			default:
				break;
		}
	}

	signal(SIGPIPE, SIG_IGN);  //игнорируем сигнал SIGPIPE

 	int efd = epoll_create(MAX_EPOLL_EVENTS);     //создаём ядерную структуру epoll

	/*
	* создаём серверный сокет и делаем его неблокирующим
	*/
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
 	if (listenfd < 0)  {
  		perror("socket");
  		return EXIT_FAILURE;
 	}
 	set_non_blocking(listenfd);

	/*
	* привязываем сокет к адресу и порту и начинаем слушать
	*/
	struct sockaddr_in servaddr = {0};
 	servaddr.sin_family = AF_INET;
 	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
 	servaddr.sin_port = htons(port);

 	if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
  		perror("bind");
  		return EXIT_FAILURE;
 	}

 	if (listen(listenfd, BACKLOG) < 0)  {
  		perror("listen");
  		return EXIT_FAILURE;
 	}

	/*
	* добавляем главный серверный сокет с список слежения epoll
	*/
	struct epoll_event listenev;
 	listenev.events = EPOLLIN | EPOLLET;
 	listenev.data.fd = listenfd;
 	if (epoll_ctl(efd, EPOLL_CTL_ADD, listenfd, &listenev) < 0) {
  		perror("epoll_ctl");
  		return EXIT_FAILURE;
 	}

	struct epoll_event connev;  //структра для сетевых соединений
 	int events_count = 1;
	//запускаем главный серверный цикл
	for(;;)	{
		//ждём сетевых соединений
		int nfds = epoll_wait(efd, events, MAX_EPOLL_EVENTS, -1);

		for (size_t i = 0; i < nfds; i++) {
			//обрабатываем серверный дескриптор
			if (events[i].data.fd == listenfd) {
    			int connfd = accept(listenfd, NULL, NULL);
    			if (connfd < 0) {      //соединились но что-то не так
     				perror("accept");
     				continue;
    			}

    			if (events_count == MAX_EPOLL_EVENTS-1) {
     				printf("Event array is full\n");
     				close(connfd);
     				continue;
    			}

				//приняли соединение, делаем его не блокирующим
				//и добавляем в пул epoll
    			set_non_blocking(connfd);
    			connev.data.fd = connfd;
    			connev.events = EPOLLIN | EPOLLOUT | EPOLLET | EPOLLRDHUP;
    			if (epoll_ctl(efd, EPOLL_CTL_ADD, connfd, &connev) < 0) {
     				perror("epoll_ctl");
     				close(connfd);
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
     				process_error(fd);

				//и рвём соединение
    			epoll_ctl(efd, EPOLL_CTL_DEL, fd, &connev);
    			close(fd);
    			events_count--;
			}
		}
		
	}

	return 0;
}

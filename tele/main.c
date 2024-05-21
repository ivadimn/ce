#include "log.h"
#include "net.h"
#include <time.h>

/*
 *  печатает help по использованию приложения
 */
void print_help(const char* app_name)
{
	printf("Использование: %s hostname \n", app_name);
}

int main(int argc, char** argv) {

	int sockfd, recv_count, send_count;
	char buf[BUF_SIZE];
	
	if (argc != 2) 	{
		print_help(argv[0]);
		exit(1);
	}
	
	sockfd = try_connect(argv[1], "31173");
	do {
	recv_count = recv(sockfd, buf, BUF_SIZE - 1, 0);
	if (recv_count == -1) {
		close(sockfd);
		err_sys("Error while reciving data\n");
	}
	
	buf[recv_count] = '\0';
	printf("client recived %d bytes: %d\n", recv_count, buf[0]);
	printf("Введите команду$ ");
	send_count = scanf("%s", buf);
	printf("client send %ld bytes: %s\n", strlen(buf), buf);
	send_count = send(sockfd, buf, strlen(buf), 0);
	if (send_count == -1) {
		close(sockfd);
		err_sys("Error while sending data");
	}
	printf("client send %d bytes\n", send_count);
	send(sockfd, "\n", 1, 0);
	

	} while(recv_count > 0);
	close(sockfd);
	return EXIT_SUCCESS;
}
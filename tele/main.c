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

	/*int status;
	struct addrinfo hints, *res, *p;
	char ipstr[INET6_ADDRSTRLEN];*/

	if (argc != 2) 	{
		print_help(argv[0]);
		exit(1);
	}
	try_connect(argv[1], "23");
		
 	/*memset(&hints, 0, sizeof hints);		// очистка структуры
	hints.ai_family = AF_UNSPEC;				// IPv4 либо IPv6
	hints.ai_socktype = SOCK_STREAM;		// потоковый сокет TCP
	

	if ((status = getaddrinfo(argv[1], NULL, &hints, &res)) != 0) {
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		exit(2);
	}
	printf("IP adresses for %s:\n\n", argv[1]);
	for (p = res; p != NULL; p = p->ai_next) {
		void* addr;
		char* ipver;
			//получит в IPv4 и IPv6 поля разные
		if (p->ai_family == AF_INET) {   //IPv4
			struct sockaddr_in *ipv4  = (struct sockaddr_in*) p->ai_addr;
			addr = &(ipv4->sin_addr);
			ipver = "IPv4";
		} else {
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6*) p->ai_addr;
			addr = &(ipv6->sin6_addr);
			ipver = "IPv6";
		}
		inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
		printf(" %s: %s\n", ipver, ipstr);
			
	}
	freeaddrinfo(res);*/
	return EXIT_SUCCESS;
}
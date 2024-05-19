#include "net.h"
#include "log.h"

int try_connect(char* hostname, char* port) {
    int status, sockfd;
	struct addrinfo hints, *res;

    memset(&hints, 0, sizeof(hints));		// очистка структуры
	hints.ai_family = AF_UNSPEC;				// IPv4 либо IPv6
	hints.ai_socktype = SOCK_STREAM;		// потоковый сокет TCP

    if ((status = getaddrinfo(hostname, port, &hints, &res)) != 0) {
		err("getaddrinfo error: %s\n", gai_strerror(status));
	}

    if (res == NULL)
        return -1;

    if ((sockfd = socket(res->ai_family, 
            res->ai_socktype, res->ai_protocol)) < 0) {
        return -1;
    }
    if (connect(sockfd, res->ai_addr, res->ai_addrlen) == 0)
    {
        printf("Соединение установлено\n");
        freeaddrinfo(res);
        shutdown(sockfd, SHUT_RDWR);
        close(sockfd);
    }
    else {
        err("connection error: %s\n", gai_strerror(status));
    }
    

    return 0;
}
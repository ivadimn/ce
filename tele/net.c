#include "net.h"
#include "log.h"


void* get_in_addr(struct sockaddr* addr) {
    if (addr->sa_family == AF_INET)  {
        return &(((struct sockaddr_in*)addr)->sin_addr);
    }
    return &(((struct sockaddr_in6*)addr)->sin6_addr);
}

int try_connect(char* hostname, char* port) {
    int status, sockfd;
	struct addrinfo hints, *res;

    memset(&hints, 0, sizeof(hints));		// очистка структуры
	hints.ai_family = AF_UNSPEC;				// IPv4 либо IPv6
	hints.ai_socktype = SOCK_STREAM;		// потоковый сокет TCP

    if ((status = getaddrinfo(hostname, port, &hints, &res)) != 0) {
		err_quit("getaddrinfo error: %s\n", gai_strerror(status));
	}

    
    if ((sockfd = socket(res->ai_family, 
            res->ai_socktype, res->ai_protocol)) < 0) {
        close(sockfd);        
        err_quit("Error open socket: \n");
    }
    if (connect(sockfd, res->ai_addr, res->ai_addrlen) == 0)
    {
        freeaddrinfo(res);
        return sockfd;
    }
    close(sockfd);
    err_sys("connection error:\n");
}
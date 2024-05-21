#ifndef _NET_H_
#define _NET_H_

#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>

#define BUF_SIZE 256


void* get_addr(struct sockaddr *addr);
 
int try_connect(char* hostname, char* port);

#endif


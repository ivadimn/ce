
CC=gcc

CFLAGS=-Wall -Wextra -Wpedantic 

all: parse

parse: main.c log.c vstr.c http.c
	$(CC) $(CFLAGS) main.c log.c vstr.c http.c -o serv

clean:
	rm -rf serv
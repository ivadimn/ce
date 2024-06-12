
CC=gcc

CFLAGS=-Wall -Wextra -Wpedantic 

all: parse

parse: main.c vstr.c 
	$(CC) $(CFLAGS) main.c vstr.c -o test

clean:
	rm -rf serv
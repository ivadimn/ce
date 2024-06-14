
CC=gcc

CFLAGS=-Wall -Wextra -Wpedantic 

all: parse

parse: main.c vstr.c str_dict.c utils.c log.c 
	$(CC) $(CFLAGS) main.c vstr.c str_dict.c utils.c log.c -o test

clean:
	rm -rf serv
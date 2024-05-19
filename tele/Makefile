
CC=gcc

CFLAGS=-Wall -Wextra -Wpedantic 

all: parse

parse: main.c log.c net.c
	$(CC) $(CFLAGS) main.c log.c net.c -o tele

clean:
	rm -rf tele
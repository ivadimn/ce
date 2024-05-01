
CC=gcc

CFLAGS=-Wall -Wextra -Wpedantic 

all: parse

parse: main.c file_info.c log.c
	$(CC) $(CFLAGS) main.c file_info.c log.c -o pth

clean:
	rm -rf pth
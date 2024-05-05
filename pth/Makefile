
CC=gcc

CFLAGS=-Wall -Wextra -Wpedantic 

all: parse

parse: main.c file_info.c log.c vstr.c dict.c utils.c sort.c
	$(CC) $(CFLAGS) main.c file_info.c log.c vstr.c dict.c utils.c sort.c  -o pth

clean:
	rm -rf pth
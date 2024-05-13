
CC=gcc

CFLAGS=-Wall -Wextra -Wpedantic 

all: parse

parse: main.c file_info.c log.c array.c utils.c sort.c dict.c
	$(CC) $(CFLAGS) main.c file_info.c log.c array.c utils.c sort.c dict.c -o pth

clean:
	rm -rf pth
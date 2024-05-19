
CC=gcc

CFLAGS=-Wall -Wextra -Wpedantic 

all: parse

parse: main.c file_info.c common.c vstr.c utils.c
	$(CC) $(CFLAGS) main.c file_info.c common.c vstr.c utils.c -o str

clean:
	rm -rf pth
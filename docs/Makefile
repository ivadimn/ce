
CC=gcc

CFLAGS=-Wall -Wextra -Wpedantic `pkg-config --cflags sqlite3 gtk+-3.0`
LIBS=`pkg-config --libs sqlite3 gtk+-3.0`

all: build

build: main.c file_info.c tv.c log.c db.c
	$(CC) $(CFLAGS) main.c file_info.c tv.c log.c db.c -o tv $(LIBS)

clean:
	rm -rf tv
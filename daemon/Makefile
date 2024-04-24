
CC=gcc

CFLAGSD=-DDAEMON -Wall -Wextra -Wpedantic -std=c11 `pkg-config --cflags sqlite3` `pkg-config --libs sqlite3`
CFLAGSC=-Wall -Wextra -Wpedantic -std=c11 `pkg-config --cflags sqlite3` `pkg-config --libs sqlite3`

daemon: dem 

dem: main.c log.c turn_daemon.c server.c params.c db.c
	$(CC) $(CFLAGSD) main.c log.c turn_daemon.c server.c params.c db.c -o dem

clean:
	rm -rf dem
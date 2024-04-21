gcc `pkg-config --cflags sqlite3` main.c log.c params.c db.c -o file_conf `pkg-config --libs sqlite3`

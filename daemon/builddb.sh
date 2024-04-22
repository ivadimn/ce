gcc `pkg-config --cflags sqlite3` maindb.c log.c params.c db.c -o conf `pkg-config --libs sqlite3`

gcc `pkg-config --cflags gtk4` main.c common.c file_info.c `pkg-config --libs gtk4 sqlite3` -o main
#!/bin/bash

# создаём файл препроцессора
gcc -Wall -E main.c -o main.i
gcc -Wall -E hash.c -o hash.i
# создаём файл ассемблера
gcc -Wall -S main.i -o main.s
gcc -Wall -S hash.i -o hash.s
# создаём объектный файл
gcc -Wall -c main.c
gcc -Wall -c hash.c
# создаём исполняемый файл
gcc -Wall main.o hash.o -o hash
 



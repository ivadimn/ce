#!/bin/bash

# создаём файл препроцессора
gcc -Wall -E main.c -o main.i
# создаём файл ассемблера
gcc -Wall -S main.c -o main.s
# создаём объектный файл
gcc -Wall -c main.c
# создаём исполняемый файл
gcc -Wall main.o -o myprog02
 



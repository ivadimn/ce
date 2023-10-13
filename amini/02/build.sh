#!/bin/bash

# создаём файл препроцессора
gcc -Wall -E ch02.c -o ch02.i
gcc -Wall -E main_ch02.c -o main_ch02.i
# создаём файл ассемблера
gcc -Wall -S ch02.i -o ch02.s
gcc -Wall -S main_ch02.i -o main_ch02.s
# создаём объектный файл
gcc -Wall -c ch02.c -o impl.o
gcc -Wall -c main_ch02.c -o main.o
# создаём исполняемый файл
#gcc -Wall ch01.o -o ch02
 



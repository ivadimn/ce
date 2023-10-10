#!/bin/bash

# создаём файл препроцессора
gcc -Wall -E ch01.c -o ch01.i
# создаём файл ассемблера
gcc -Wall -S ch01.i -o ch01.s
# создаём объектный файл
gcc -Wall -c ch01.c
# создаём исполняемый файл
gcc -Wall ch01.o -o ch01
 



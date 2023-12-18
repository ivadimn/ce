#!/bin/bash

gcc main.c sqlite3.c -lpthread -ldl -lm -o main

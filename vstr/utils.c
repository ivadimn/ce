#include "utils.h"
#include "vstr.h"
#include "common.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>

const char chex[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF};

static long inhex(char ch) {
	const char* hexch = "0123456789ABCDEF";
	for (long i = 0; i < 16; i++)  {
        if (hexch[i] == ch)
            return i;
    }
    return -1;
}

/*
* разбивает строку на подстроки по разделителю delim и вывделяет группы
* группа символов начинается g_open символом и заканчиваетяс g_close символом
* номера индексов сиволов открытия и закрытия группы должны совпадать в g_open
* и g_close
*/
void split(char** arr, char* str, char delim, char* g_open, char* g_close) {
    long len = strlen(str); 
    long part_index = 0, arr_index = 0;
    int  in_group = 0;
    long g_index = -1;

    char part[MAX_PART];
    
    for (long i = 0; i < len; i++) {
        
        switch (in_group)  {
            case 0:
                if (str[i] == delim) {
                    part[part_index] = '\0';
                    strcpy(arr[arr_index++], part);
                    part_index = 0;
                    
                } 
                else if(g_open && (g_index = str_in(g_open, str[i])) >= 0) {
                    in_group = 1;
                }
                else {
                    part[part_index++] = str[i];
                }
                break;
            case 1:
                if (str[i] == g_close[g_index]) {
                    in_group = 0;
                    g_index = -1;
                }
                else {
                    part[part_index++] = str[i];
                }
                break;
            default:
                break;
        }
    }
    part[part_index] = '\0';
    strcpy(arr[arr_index], part);
}

/*
* возвращает первое вхождение символа в строку
*/
long str_in(char *str, char ch) {
    long len = (long)strlen(str);
    for (long i = 0; i < len; i++)  {
        if (str[i] == ch)
            return i;
    }
    return -1;
}

long str_instr(char *str, char* s) {
    long len_s = (long)strlen(s);
    long len_str = (long)strlen(str);
    if (len_str < len_s)
        return -1;
    
    for (long index = 0; index < len_str - len_s; index++) {
        if (strncmp(&str[index], s, (size_t) len_s) == 0)
            return index;
    }
    return -1;    
}

char* str_substr(char *str, long start, long end) {
    long len = end - start;
    char* substr = NULL;
    if(len < 1)
        return NULL;
    substr = (char*) alloc(sizeof(char) * (len + 1));    
    for (long i = 0; i < len; i++) {
        substr[i] = str[start + i];
    }
    substr[len] = '\0';
    return substr;
}

void str_urldecode(char *str) {
    char buf[4096];
	char ch;
	state_t state = ONE;
	long index, buf_index = 0;
    int decode = 0;
    long len = strlen(str);

    for (long i = 0; i < len; i++) {

		if (str[i] == '%') {
            decode = 1;
			continue;
        }
		index = inhex(str[i]);
		if (index >= 0 && decode) {
			switch (state) {
				case ONE:
					ch = chex[index];
					ch <<= 4;
					state = TWO;
					break;
				case TWO:
					ch |= chex[index];
					state = ONE;
					//buf[buf_index++] = ch;
                    str[buf_index++] = ch;
                    decode = 0;
					break;
				default:
					break;
			}
		}
		else {
			//buf[buf_index++] = str[i];
            str[buf_index++] = str[i];
		}
	}
	//buf[buf_index++] = '\0';
    //strcpy(str, buf);
    str[buf_index++] = '\0';
}

void* alloc(size_t size) {
    void *p = malloc(size);
    
    if (p == NULL)
        err_sys("Ошибка распределения памяти: ");
    return p;
}



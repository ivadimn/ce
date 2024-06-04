#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "log.h"




static struct option options[] = {
		{"help", no_argument, 0, 'h'},
		{"dbms", required_argument, 0, 'b'},
    	{"dbname", required_argument, 0, 'd'},
		{"table", required_argument, 0, 't'},
		{"column", required_argument, 0, 'c'},
		{NULL, 0, 0, 0}
};

/*
 *  печатает help по использованию приложения
 */
void print_help(char* app_name)
{
	printf("\n Использование: %s ОПЦИИ\n\n", app_name);
	printf("  Options:\n");
	printf("   -h --help                 		Печатает эту справку\n");
	printf("   -p --port=PORT_NUMBER		 	Номер порта\n");
	printf("   -d --dbname=DATABASE_NAME 		Наименование базы данных\n");
	printf("   -t --table=TABLE_NAME     		Наименование таблицы\n");
  	printf("   -с --column=COLUMN_NAME   		Наименование колонки\n");
	printf("\n");
}

int get_line(char* line, int size) {
    char ch;
    int index = 0;
    while ((ch = getchar()) != '\n' && index < size - 1) {
        line[index++] = ch;
    }
    line[index] = '\0';    
    return index - 1;
}



int main (int argc,char **argv)
{
	int value, option_index = 0;
	char port[6];

	if (argc == 1) 	{
		print_help(argv[0]);
		return EXIT_FAILURE;
	}
	
	if (argc < 7) 	{
		print_help(argv[0]);
		return EXIT_FAILURE;
	}

	while ((value = getopt_long(argc, argv, "hb:d:t:c:", options, &option_index)) != -1) {
		switch (value) {
			case 'h':
				print_help(argv[0]);
				return EXIT_FAILURE;
			case 'p':
				strcpy(port, optarg);
				break;
			case '?':
				print_help(argv[0]);
				return EXIT_FAILURE;
			default:
				break;
		}
	}

				
	return 0;
}

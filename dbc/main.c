#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "db.h"
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
	printf("   -b --dbms=SQLITE | POSTGRESQL 	Наименование базы данных\n");
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

void get_conn_info(conninfo_t* conninfo) {
	printf("Введите IP адрес сервера: ");
	get_line(conninfo->hostaddr, IP4_LEN);
	printf("Введите имя пользователя: ");
	get_line(conninfo->user, INFO_LEN);
	printf("Введите пароль: ");
	get_line(conninfo->password, INFO_LEN);
}

void handle_sqlite(conninfo_t* conninfo, const char* table, const char* column) {
	double r_avg = 0;
	int result;
	open_db(SQLITE, conninfo);
	is_valid_column_type(table, column);
	result = avg(table, column, &r_avg);
	printf("Result = %d\n", result);
	close_db();
}

void handle_psql(conninfo_t* conninfo, const char* table, const char* column) {
	double r_avg = 0;
	int result;
	open_db(POSTGRESQL, conninfo);
	result = avg(table, column, &r_avg);
	printf("Result = %d\n", result);
	close_db();
}


int main (int argc,char **argv)
{
	int value, option_index = 0;
	conninfo_t conninfo;
	char dbms[INFO_LEN];
	char table[NAME_LEN];
	char column[NAME_LEN];

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
			case 'b':
				strcpy(dbms, optarg);
				info("Опция -b со значением: %s", optarg);
				break;
			case 'd':
				strcpy(conninfo.dbname, optarg);
				info("Опция -d со значением: %s", optarg);
				break;
			case 't':
				strcpy(table, optarg);
				info("Опция -t со значением: %s", optarg);
				break;
			case 'c':
				strcpy(column, optarg);
				info("Опция -c со значением: %s", optarg);
				break;	
			case '?':
				print_help(argv[0]);
				return EXIT_FAILURE;
			default:
				break;
		}
	}
	if (strcmp(dbms, "SQLITE") == 0) {
		handle_sqlite(&conninfo, table, column);
	}
	else if (strcmp(dbms, "POSTGRESQL") == 0) {
		get_conn_info(&conninfo);
		handle_psql(&conninfo, table, column);
	}
		
	return 0;
}

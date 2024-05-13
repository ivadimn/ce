#include "log.h"
#include "utils.h"
#include "file_info.h"
#include <time.h>
#include <pthread.h>

/*
 *  печатает help по использованию приложения
 */
void print_help(const char* app_name)
{
	printf("\n Использование: %s <кол-во потоков>  <каталог с лог файлами> \n", app_name);
}

int main(int argc, char** argv) {

	time_t begin, end;
	size_t count = 0;
	char** flist = NULL;
	unsigned char** hosts = NULL;
	long *bytes = NULL;
	size_t num_hosts = 0, num_refers = 0;

    if (argc < 3) {
        print_help(argv[0]);
		exit(1);
	}
    
	count = get_count_files(argv[2]);
	if (count == 0)
		return EXIT_FAILURE;
	
	flist = init_file_list(count);
	file_list(argv[2], flist);
	for (size_t i = 0; i < count; i++) {
		printf("%s\n", flist[i]);
	}

	init_arrs();
	//init_dicts();
	
	//for (size_t i = 0; i < count; i++) {
		printf("Обрабатываем Файл %s\n", flist[0]);	
		begin = time(NULL);
		handle_file1(flist[0]);
		end = time(NULL);
		printf("Файл %s обработан за время: %lf.\n", flist[0], difftime(end, begin));	
	//}
	
	//printf("Time interval %lf\n", difftime(end, begin));
	
	
	num_hosts = get_hosts(&hosts, &bytes);
	if (hosts && bytes) 	{
		for (size_t i = num_hosts - 1; i >= num_hosts - 10; i--)	{
			printf("host: %s - %ld\n", hosts[i], bytes[i]);
		}	
	}
	
	remove_hosts(&hosts, &bytes);

	free_file_list(flist, count);
	remove_arrs();
	//remove_dicts();
	
	return EXIT_SUCCESS;

}
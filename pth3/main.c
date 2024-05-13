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

	pthread_t *thread;
    size_t thindex = 0;
    size_t thcount = 0;
    size_t file_index = 0;
	size_t file_count = 0;
	char** flist = NULL;
	unsigned char** hosts = NULL;
	size_t *bytes = NULL;
	size_t num_hosts = 0, num_refers = 0;
	int result;

    if (argc < 3) {
        print_help(argv[0]);
		exit(1);
	}
	   
	file_count = get_count_files(argv[2]);
	if (file_count == 0)
		crit("Каталог: %s не содержит файлов\n", argv[2]);

	thcount = strtoul(argv[1], NULL, 10);
	if (thcount == 0)
		thcount = file_count;
	
    thread = (pthread_t*) alloc(sizeof(pthread_t) * thcount);
	
	flist = init_file_list(file_count);
	file_list(argv[2], flist);
	for (size_t i = 0; i < file_count; i++) {
		printf("%s\n", flist[i]);
	}

	init_mtx();

	while(1) {

        for (thindex = 0; thindex < thcount; thindex++) {
            if (file_index >= file_count)
                break;
            result = pthread_create(&thread[thindex], NULL, handle_file, flist[file_index++]);
            printf("Поток создан thread[%ld] = %lu\n", thindex, thread[thindex]);
            if (result)  {
                crit("Поток не может быть создан, ошибка: %d\n", result);
            }
        }
        for (size_t j = 0; j < thindex; j++)  {
            result = pthread_join(thread[j], NULL);
            if (result)  {
                crit("Поток не может быть присоединён, ошибка: %d\n", result);
            }
        }
        if (file_index >= file_count)
            break;
    }
	
	release_mtx();	

	num_hosts = get_hosts(&hosts, &bytes);
	printf("10 самых \"тяжёлых\" по трафику URL'ов\n");
	if (hosts && bytes) 	{
		for (size_t i = num_hosts - 1; i >= num_hosts - 1000; i--)	{
			printf("host: %s - %ld\n", hosts[i], bytes[i]);
		}	
	}
	
	remove_hosts(&hosts, &bytes);

	free_file_list(flist, file_count);
	remove_trees();
		
	return EXIT_SUCCESS;

}
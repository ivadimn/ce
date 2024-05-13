#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>

char* files[] = {
    "file0",
    "file1",
    "file2",
    "file3",
    "file4",
    "file5",
    "file6",
    "file7",
    "file8",
    "file9"
};

size_t all_len = 0;

void print_help(const char* app_name)
{
	printf("\n Использование: %s <кол-во потоков>  <каталог с лог файлами> \n", app_name);
}

void* thread_body(void* arg) {
    char* f = (char*)arg;
    size_t len = strlen(f);
    all_len += len;
    printf("Файл %s обработан\n", f);
    return NULL;
}

int main(int argc, char** argv) {

    //обработчик потоков
    pthread_t *thread;
    long thindex = 0;
    long thcount = 0;
    long data_index = 0;
    int result = 0;
    size_t i = 0;

    if (argc < 2) {
        print_help(argv[0]);
		exit(1);
	}
    thcount = strtol(argv[1], NULL, 10);
    thread = (pthread_t*) malloc(sizeof(pthread_t) * thcount);

    while(1) {

        for (thindex = 0; thindex < thcount; thindex++) {
            if (data_index >= 10)
                break;
            result = pthread_create(&thread[thindex], NULL, thread_body, files[data_index++]);
            printf("Поток создан thread[%ld] = %lu\n", thindex, thread[thindex]);
            if (result)  {
                printf("Поток не может быть создан, ошибка: %d\n", result);
                exit(1);
            }
        }
        for (size_t j = 0; j < thindex; j++)  {
            result = pthread_join(thread[j], NULL);
            if (result)  {
                printf("Поток не может быть присоединён, ошибка: %d\n", result);
                exit(2);
            }
        }
        printf("----------------------------------------------------\n");
        if (data_index >= 10)
            break;
    }
    
    free(thread);
    return 0;
    
}
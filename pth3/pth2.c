/*
* отсоединенный поток
*/

#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

void* thread_body(void* arg) {
    printf("Привет мир из другого отсоединенного потока\n");
    return NULL;
}

int main(int argc, char** argv) {

    //обработчик потоков
    pthread_t thread;

    int result = pthread_create(&thread, NULL, thread_body, NULL);
    if (result)  {
        printf("Поток не может быть создан, ошибка: %d\n", result);
        exit(1);
    }

    result = pthread_detach(thread);
    if (result)  {
        printf("Поток не может быть отсоединён, ошибка: %d\n", result);
        exit(2);
    }

    pthread_exit(NULL);

    return 0;
    
}
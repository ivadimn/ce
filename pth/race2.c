/*
* демонстрация состояния гонки с разделеием данных
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>

void* thread_body_1(void* arg) {
    //указатель на разделяемую переменную
    int *shared_var_ptr = (int*) arg;
    for (size_t i = 0; i < 10000; i++)  {
        (*shared_var_ptr)++;    
    }
    
    printf("%d\n", *shared_var_ptr);
    return NULL;
} 

void* thread_body_2(void* arg) {
    //указатель на разделяемую переменную
    int *shared_var_ptr = (int*) arg;
    for (size_t i = 0; i < 10000; i++)  {
        (*shared_var_ptr) += 2;    
    }
    printf("%d\n", *shared_var_ptr);
    return NULL;
}

int main(int argc, char** argv) {

    int shared_var = 0;
    //обработчик потоков
    pthread_t thread1;
    pthread_t thread2;
    
    int result1 = pthread_create(&thread1, NULL, thread_body_1, &shared_var);
    int result2 = pthread_create(&thread2, NULL, thread_body_2, &shared_var);
    
    if (result1 || result2)  {
        printf("Потоки не могут быть созданы\n");
        exit(1);
    }

    result1 = pthread_join(thread1, NULL);
    result2 = pthread_join(thread2, NULL);
    
    if (result1 || result2)  {
        printf("Потоки не могут быть присоединены\n");
        exit(2);
    }

    return 0;
    
}
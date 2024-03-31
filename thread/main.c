#include <pthread.h> 
#include <stdio.h>

void* thread_func(void* arg) {
    printf("Child thread running %d\n", *(int*)arg);
    return (void*)-1;
} 

int main() {
    printf("Main thread running\n");
    pthread_t pid;
    int result = 1001;
    pthread_create(&pid, NULL, thread_func, (void*)&result);
    pthread_join(pid, (void**) &result);
    printf("Child thread returning - %d\n", result);
    return 0;
}

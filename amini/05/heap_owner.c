#include <stdio.h>
#include <stdlib.h>

#define QUEUE_MAX_SIZE 100

typedef struct {
    int front;
    int rear;
    double* arr;
} queue_t;

void init(queue_t* q) {
    q->front = q->rear = 0;
    //выделенными здесь блоками кучи владеет объект очередт
    q->arr = (double*) malloc(QUEUE_MAX_SIZE * sizeof(double));
}

void destroy(queue_t* q) {
    free(q->arr);
}

int size(queue_t* q) {
    return q->rear - q->front;
}

void enqueue(queue_t* q, double item) {
    q->arr[q->rear] = item;
    q->rear++;
}

double dequeue(queue_t* q) {
    double item = q->arr[q->front];
    q->front++;
    return item;
}

int main(int argc, char** argv) {
    queue_t* q = (queue_t*)malloc(sizeof(queue_t));
    init(q);

    enqueue(q, 6.5);
    enqueue(q, 1.3);
    enqueue(q, 2.4);

    printf("%f\n", dequeue(q));
    printf("%f\n", dequeue(q));
    printf("%f\n", dequeue(q));

    destroy(q);
    free(q);

    return 0;
}
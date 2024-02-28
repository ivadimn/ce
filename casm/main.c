#include <stdio.h>
#include <stdlib.h>


char *int_format = "%ld ";
long data[] = {4, 8, 15, 16, 23, 42};
const int data_length = 6;

typedef void (*print_func)(long);

typedef struct list_node_t {
    long value;
    struct list_node_t* next;
} list_node_t;

void print_int(long value) {
    printf(int_format, value);
}

void m(list_node_t* node, print_func pfunc) {

    if (node == NULL)
        return;
    pfunc(node->value);
    node = node->next;
    m(node, pfunc);
}

list_node_t* add_element(list_node_t* head, long value) {

    list_node_t *node = (list_node_t *) malloc(sizeof(list_node_t));
    node->value = value;
    node->next = head;
    return node;
}

void view(list_node_t* head) {
    list_node_t* lhead = head;
    while(lhead) {
        print_int(lhead->value);
        lhead = lhead->next;
    }
}

int main(int argc, char** argv) {

    list_node_t* head = NULL;
    int index = data_length - 1;
    while(index >= 0) {
        head = add_element(head, data[index]);
        index--;
    }
    m(head, print_int);
    printf("\n");
    return 0;

}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/*
#define PRINT(a) printf("%d\n", a);
#define LOOP(v, s, e) for (int v = s; v <= e; v++) {
#define ENDLOOP }
*/

#define CMD(NAME) \
    char NAME ## _cmd[256] = ""; \
    strcpy(NAME ## _cmd, #NAME); 

#define VERSION "2.3.4"    

#define LOG_ERROR(format, ...) \
    fprintf(stderr, format, __VA_ARGS__);    

#define LOOP_3(X, ...) \
    printf("%s\n", #X);

#define LOOP_2(X, ...) \
    printf("%s\n", #X); \
    LOOP_3(__VA_ARGS__)  

#define LOOP_1(X, ...) \
    printf("%s\n", #X); \
    LOOP_2(__VA_ARGS__)

#define LOOP(...) \
    LOOP_1(__VA_ARGS__)  

#define SIZE 5    


void print_bytes(void* data, size_t lenght) {
    char delim = ' ';
    unsigned char* ptr = data;

    for (size_t i = 0; i < lenght; i++) {
        printf("%c 0x%x", delim, *ptr);
        delim = ',';
        ptr++;
    }
    printf("\n");
}

int* create_an_integer(int default_value) {
    int* var_ptr = (int*) malloc(sizeof(int));
    *var_ptr = default_value;
    return var_ptr;     
} 



int main(int argc, char** argv) {

   /* LOOP(counter, 1, 10)
        PRINT(counter)
    ENDLOOP 

    CMD(copy)
    CMD(paste)
    CMD(cut)

    char cmd[256];
    
    printf("copy_cmd: %s\n", copy_cmd);
    printf("paste_cmd: %s\n", paste_cmd);
    printf("cut_cmd: %s\n", cut_cmd);

    printf("Напишите что-нибудь: ");
    scanf("%s", cmd);

    printf("cmd: %s\n", cmd);

    if(strcmp(cmd, copy_cmd) == 0){
        printf("-%s\n", cmd);
    }    
    if(strcmp(cmd, paste_cmd) == 0){
        printf("-%s\n", cmd);
    }
    if(strcmp(cmd, cut_cmd) == 0){
        printf("-%s\n", cmd);
    } 

    if (argc < 3) {
        LOG_ERROR("Invalid number of arguments for version %s\n", VERSION);
        exit(1);
    }
    
    if (strcmp(argv[1],  "-n") != 0) {
        LOG_ERROR("%s is a wrong at index at %d for version %s\n", argv[1], 1,  VERSION);
        exit(1);
    }
    */
    /*LOOP(copy paste cut)
    LOOP(copy, paste, cut)
    LOOP(copy, paste, cut, select)*/
    /*int var = 1;
    int* int_ptr = NULL;
    int_ptr = &var;
    
    char* char_ptr = NULL;
    char_ptr = (char*)&var;

    printf("Before arithmetic: int_ptr: %p, char_ptr: %p\n",
        int_ptr, char_ptr );

    int_ptr++;
    char_ptr++;    

    printf("After arithmetic: int_ptr: %p, char_ptr: %p\n",
        int_ptr, char_ptr );

    int arr[SIZE] = {9, 22, 30, 23, 18};
    for(int i = 0; i < SIZE; i++) {
        printf("%d\n", arr[i]);
    }    

    int *ptr = &arr[0];
    for(;;) {
        printf("%d\n", *ptr);
        if (ptr == &arr[SIZE - 1]) {
            break;
        }
        ptr++;
    } 
    int a = 9;
    double b = 18.9;

    print_bytes(&a, sizeof(int));
    print_bytes(&b, sizeof(double));*/

    int* ptr = NULL;
    ptr = create_an_integer(10);

    printf("%d\n", *ptr);
    free(ptr);

    return 0;
}
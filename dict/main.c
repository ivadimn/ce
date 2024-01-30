#include "common.h"
#include "dict.h"

int main(int argc, char** argv) {
    
    if (argc < 2)  {
        err_msg("Не достаточно аргументов!");
        exit(1);
    }
    
    file_handle(argv[1]);
    return EXIT_SUCCESS;
    
}
#include "common.h"

int main(int argc, char** argv) {

    if (argc != 4) {
        err_cont("Не правильное количество аргументов.");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
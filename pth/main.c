#include "log.h"
#include "file_info.h"

/*
 *  печатает help по использованию приложения
 */
void print_help(const char* app_name)
{
	printf("\n Использование: %s <кол-во потоков>  <каталог с лог файлами> \n", app_name);
}

int main(int argc, char** argv) {

	size_t count = 0;
	char** flist;

    if (argc < 3) {
        print_help(argv[0]);
		exit(1);
	}
    
	count = get_count_files(argv[2]);
	if (count == 0)
		return EXIT_FAILURE;
	
	flist = init_file_list(count);
	file_list(argv[2], flist);
	for (size_t i = 0; i < count; i++) {
		printf("%s\n", flist[i]);
	}
	handle_file(flist[0]);

	return EXIT_SUCCESS;

}
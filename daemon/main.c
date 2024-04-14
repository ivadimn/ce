#include "common.h"
#include "server.h"
#include "turn_daemon.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <getopt.h>

static char *app_name = NULL;
int log_to_stderr;

/*
 *  печатает help по использованию приложения
 */
void print_help(void)
{
	printf("\n Использование: %s [ОПЦИИ]\n\n", app_name);
	printf("  Options:\n");
	printf("   -h --help                 Печатает эту справку\n");
	printf("   -d --daemon               Запукск в режиме демона\n");
	printf("   -c --console              Запуск в интерактивном режиме\n");
	printf("\n");
}

int main(int argc, char** argv) {

    static struct option options[] = {
		{"daemon", no_argument, 0, 'd'},
		{"console", no_argument, 0, 'c'},
		{"help", no_argument, 0, 'h'},
		{NULL, 0, 0, 0}
	};

    int value, option_index = 0;
    int is_daemon = 0;
    app_name = argv[0];
    log_to_stderr = 1;
    if (argc < 2)
        print_help();

    while ((value = getopt_long(argc, argv, "hdc", options, &option_index)) != -1) {
		switch (value) {
			case 'c':
				is_daemon = 0;
                printf("Интерактивный режим \n");
				break;
			case 'd':
				is_daemon = 1;
                printf("Режим демона\n");
				break;
			case 'h':
				print_help();
				return EXIT_SUCCESS;
			case '?':
				print_help();
				return EXIT_FAILURE;
			default:
				break;
		}
	}    
    if (is_daemon) {
        log_to_stderr = 0;
        turn_daemon(app_name);    
    }

    run_server(app_name);

    return EXIT_SUCCESS;

}
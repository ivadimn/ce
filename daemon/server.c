#include "common.h"
#include "server.h"
#include <sys/types.h>
#include <fcntl.h>


int run_server(const char* cmd) {

    int sock, client_sock, rval;
    struct sockaddr_un server;
    char buf[1024];

    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    log_info("Начало работы сервера %s\n", cmd);
    if (sock < 0) 
        log_sys("Ошибка при открытии сокета: %s", SOCKET_NAME);
    

    server.sun_family = AF_UNIX;
    strcpy(server.sun_path, SOCKET_NAME);

    if (bind(sock, (struct sockaddr *) &server, sizeof(struct sockaddr_un))) {
        log_sys("Ошибка привязки сокета: %s", SOCKET_NAME);
    }

    log_info("Сокет имеет имя %s, %s\n", server.sun_path, cmd);
    
    listen(sock, 5);

    for (;;) {
        client_sock = accept(sock, 0, 0);
        if (client_sock == -1)
        {
            log_ret("Ошибка вызова функции accept.");
            break;
        }
        else do {
            bzero(buf, sizeof(buf));
            
            if ((rval = read(client_sock, buf, 1024)) < 0)
                log_ret("Ошибка чтания данных из сокета.");
            else if (rval == 0)
                log_ret("Закрытие соединения.");
            else
                send(client_sock, buf, strlen(buf), 0);
        } while (rval > 0);

        close(client_sock);
    }
    close(sock);
    unlink(SOCKET_NAME);
    log_info("Окончание работы сервера %s\n", cmd);
    return 0;
}


int64_t get_file_size(const char* filename) {
    
	int fd;
    int64_t fsize = 0;
	struct stat fileStatbuff;

    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        log_ret("Ошибка открытия файла: %s", filename);
        return -1;
    }
	if ((fstat(fd, & fileStatbuff) != 0) || (!S_ISREG(fileStatbuff.st_mode))) {
		fsize = -1;
	}
	else{
		fsize = fileStatbuff.st_size;
	}
    close(fd);
	return fsize;
}

 


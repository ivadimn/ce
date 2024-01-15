#include "common.h"
#include "zip.h"

char buf1[] = "abcdefghij";
char buf2[] = "ABCDEFGHIJ";
char digits[] = "0123456789";

int main(int argc, char** argv) {
    
    ecodr_t* ecodr = create_ecodr();
    /*int fd;
    if ((fd = creat("file.hole", FILE_MODE)) < 0)
        err_sys("Ошибка вызова create");

    if (write(fd, buf1, 10) != 10)
        err_sys("Ошибка записи buf1");

    if (lseek(fd, 1024, SEEK_SET) == -1)
        err_sys("Ошибка вызова lseek");

    if (write(fd, buf2, 10) != 10)
        err_sys("Ошибка записи buf2");

    close(fd);*/
    if (argc < 2) {
        err_msg("Недостаточно аргументов!");
        return 1;
    }
    
    

    /*int fd = open(argv[1], O_RDONLY);
    if (fd == -1)
        err_sys("Ошибка открытия файла: %s", argv[1]);

    zip_header_t local_header;
    if (read(fd, &local_header, 30) != 30)
        err_sys("Ошибка чтения файла: %s", argv[1]);       

    
    printf("Размер заголовка: %ld\n", sizeof(zip_header_t));
    zip_time_t zt;
    zt = get_zip_time(local_header.modification_time);
    zip_date_t zd = get_zip_date(local_header.modification_date);
    print_rawzip_info(local_header);
    printf("Время модифиекации: %2u:%2u:%2u\n", zt.hours, zt.minutes, zt.seconds);
    printf("Дата модифиекации: %2u:%2u:%4u\n", zd.day, zd.month, zd.year);
    close(fd); */

    int iz = zip_preview(argv[1], ecodr);
    if (iz < 0) {
        printf("Файл %s не содержит архива.\n", argv[1]);
    }
    else if (iz == 0)
    {
        printf("Файл %s содержит пустой архив.\n", argv[1]);
    }
    else {
        printf("Файл %s содержит архив.\n", argv[1]);
        print_ecodr_info(ecodr);
        zip_contains(argv[1], ecodr);
    }
  
    delete_ecodr(ecodr);
    return 0;
}

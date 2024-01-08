#include "common.h"
#include "zip.h"
#include <stdio.h>
#include <fcntl.h>

char buffer[BUF_SIZE];



int get_str_time(char* str_time, uint16_t r_time) {
    int buf_size = 8;
    int count = snprintf(str_time, sizeof(char) * buf_size + 1, "%2d:%2d:%2d", 
            HOURS(r_time), MINUTES(r_time), SECONDS(r_time));
    if (count < 0) {
        err_cont("Ошибка: ");
    }

    for (int i = 0; i < buf_size; i++) {
        if(str_time[i] == ' ')
            str_time[i] = '0';
    }
    return 1;
}

int get_str_date(char* str_date, uint16_t r_date) {
    int buf_size = 10;
    int count = snprintf(str_date, sizeof(char) * buf_size + 1, "%2d.%2d.%d", 
            DAY(r_date), MONTH(r_date), YEAR(r_date));
    if (count < 0) {
        err_cont("Ошибка: ");
    }
    
    for (int i = 0; i < buf_size; i++) {
        if(str_date[i] == ' ')
            str_date[i] = '0';
    }
    return 1;
}

void print_file_info(cfh_short_t header) {
    char time[8];
    char date[10];
    
    printf("*************************************************\n");
    printf("Версия: %x\n", header.version);
    printf("Флаги: %x\n", header.flags);
    printf("Метод сжатия: %x\n", header.compression);

    get_str_time(time, header.modification_time);
    printf("Время модификации: %s\n", time);

    get_str_date(date, header.modification_date);
    printf("Дата модификации: %s\n", date);

    printf("CRC: %x\n", header.crc);
    printf("Сжатый размер: %x\n", header.compressed_size);
    printf("Несжатый размер: %x\n", header.uncompressed_size);
    printf("Длина имени файла: %x\n", header.filename_len);
    printf("Дополнительная длина поля: %x\n", header.extrafield_len);
    printf("*************************************************\n");
}

int zip_contains(char* filename) {
    
    uint32_t sig;
    cfh_short_t zh;
    int result = -1;
    int index = 1;

    int fd = open(filename, O_RDONLY);
    if (fd == -1)
        err_sys("Ошибка открытия файла: %s", filename);

    while( read(fd, &sig, sizeof(uint32_t)) == sizeof(uint32_t)) {
        if (sig == CFH_SIG) {
            /*printf("sig = %x\n", sig);
            result = 1;
            break;*/
            if (read(fd, &zh, sizeof(cfh_short_t)) == sizeof(cfh_short_t)) {
                read(fd, &buffer, sizeof(char) * zh.filename_len);
                buffer[zh.filename_len] = '\0';
                if (zh.external_attr & 0x10)
                    printf("Имя файла: %s, это каталог\n", buffer);
                else {    
                    printf("%d. Имя файла: %s\n", index++, buffer);
                    print_file_info(zh);    
                }    

            }
        }
        else {
            lseek(fd, -(sizeof(uint32_t) - sizeof(uint8_t)), SEEK_CUR);
        }
    }
    close(fd);
    return result;
}

int zip_preview(char* filename) {
    uint32_t sig;
    int cfh_count = 0;
    off_t off = 4;
    int fd = open(filename, O_RDONLY);

    if (fd == -1)
        err_sys("Ошибка открытия файла: %s", filename);    

    off_t eof = lseek(fd, off, SEEK_END);
    printf("off_t %ld\n", eof);
    while(eof > 0) {
        read(fd, &sig, sizeof(uint32_t));
        if (sig == CFH_SIG) {
            printf("founded off_t %ld\n", eof);
            break;
        }
        else {
            lseek(fd, -(off + 1), SEEK_CUR);
        }
    }
    close(fd);

    return cfh_count;    
}

#include "common.h"
#include "zip.h"
#include <fcntl.h>

char buffer[BUF_SIZE];

/*
Функции перевода времени и дата в текстовую строку
*/
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
//********************************************************************************

/*
Функции работы со структурой конца записи центрального каталога
*/
ecodr_t* create_ecodr() {
    ecodr_t* ecodr;
    ecodr = (ecodr_t*) malloc(sizeof(ecodr_t));
    ecodr->comment = NULL;
    return ecodr;
}

void delete_ecodr(ecodr_t* ecodr) {
    if(ecodr->comment != NULL)
        free(ecodr->comment);
    free(ecodr);   
}

void add_comment(ecodr_t* ecodr, char* comment) {
    ecodr->comment = (char*) malloc(strlen(comment));
    memcpy(ecodr->comment, comment, strlen(comment));
}

void print_ecodr_info(ecodr_t* ecodr) {
    in_ecodr_t* inecodr = &(ecodr->ecodr);
    printf("Номер этого диска: %u\n", inecodr->disk_number);
    printf("Номер диска, на котором начинается центральный каталог: %u\n", 
            inecodr->cfh_start_disk);
    printf("Количество записей центрального каталога на диске: %u\n", 
            inecodr->cfh_disk_count);
    printf("Всего записей центрального каталога: %u\n", inecodr->cfh_count);
    printf("Размер центрального каталога: %u\n", inecodr->cfh_size);
    printf("Смещение центрального каталога на диске: %x\n", 
            inecodr->cfh_offset);
    if (ecodr->comment != NULL) {
        printf("Комментарий: %s\n", ecodr->comment);    
    }
}
//********************************************************************************** 


/*
Функции работы с записью центрального каталога
*/
cfh_t* create_cfh(){
    cfh_t* cfh = (cfh_t*) malloc(sizeof(cfh_t));
    cfh->filename = NULL;
    cfh->filecomment = NULL;
    cfh->extra = NULL;
}

void delete_cfh(cfh_t* cfh) {
    if (cfh->filename != NULL)
        free(cfh->filename);
    if (cfh->filecomment != NULL)
        free(cfh->filecomment);
    if (cfh->extra != NULL)
        free(cfh->extra);
    free(cfh);            
}

void add_file(cfh_t* cfh, char* filename) {
    cfh->filename = (char*) malloc(strlen(filename));
    memcpy(cfh->filename, filename, strlen(filename));
}

void add_filecomment(cfh_t* cfh, char* filecomment) {
    cfh->filecomment = (char*) malloc(strlen(filecomment));
    memcpy(cfh->filecomment, filecomment, strlen(filecomment));
}
void add_extradata(cfh_t* cfh, uint8_t* extra) {
    cfh->extra = (uint8_t*) malloc(sizeof(uint8_t) * cfh->cfh.extrafield_len);
    memcpy(cfh->extra, extra, cfh->cfh.extrafield_len);
}


void print_file_info(cfh_t* cfh) {
    char time[8];
    char date[10];
    cfh_short_t* header = &(cfh->cfh);

    printf("*************************************************\n");
    printf("Версия: %x\n", header->version);
    printf("Флаги: %x\n", header->flags);
    printf("Метод сжатия: %x\n", header->compression);

    get_str_time(time, header->modification_time);
    printf("Время модификации: %s\n", time);

    get_str_date(date, header->modification_date);
    printf("Дата модификации: %s\n", date);

    printf("CRC: %x\n", header->crc);
    printf("Сжатый размер: %x\n", header->compressed_size);
    printf("Несжатый размер: %x\n", header->uncompressed_size);
    printf("Длина имени файла: %x\n", header->filename_len);
    printf("Дополнительная длина поля: %x\n", header->extrafield_len);
    printf("*************************************************\n");
}
//*************************************************************************************************


int zip_contains(char* filename, ecodr_t* ecodr) {
    
    uint32_t sig = 0;
    cfh_short_t zh;
    int result = -1;
    int index = 1;
    off_t off = 0;

    int fd = open(filename, O_RDONLY);
    if (fd == -1)
        err_sys("Ошибка открытия файла: %s", filename);

    read(fd, &sig, sizeof(uint32_t)); 
    while(sig != LFH_SIG) {
        off = lseek(fd, -(sizeof(uint32_t) - sizeof(uint8_t)), SEEK_CUR);
        read(fd, &sig, sizeof(uint32_t));
    }
    
    printf("Нашли Local File Header");
    off = lseek(fd, (ecodr->ecodr.cfh_offset - sizeof(uint32_t)), SEEK_CUR);
    read(fd, &sig, sizeof(uint32_t));
    printf("Current SIG: %x\n", sig);
    /*while( read(fd, &sig, sizeof(uint32_t)) == sizeof(uint32_t)) {
        if (sig == CFH_SIG) {
            printf("sig = %x\n", sig);
            result = 1;
            break;
            if (read(fd, &zh, sizeof(cfh_short_t)) == sizeof(cfh_short_t)) {
                read(fd, &buffer, sizeof(char) * zh.filename_len);
                buffer[zh.filename_len] = '\0';
                if (zh.external_attr & 0x10)
                    printf("Имя файла: %s, это каталог\n", buffer);
                else {    
                    printf("%d. Имя файла: %s\n", index++, buffer);
                    //print_file_info(zh);    
                }    

            }
        }
        else {
            lseek(fd, -(sizeof(uint32_t) - sizeof(uint8_t)), SEEK_CUR);
        }
    }*/
    close(fd);
    return result;
}

int zip_preview(char* filename, ecodr_t *ecodr)  {
    uint32_t sig;
    int cfh_count = -1;
    in_ecodr_t *in_ecodr = &(ecodr->ecodr);
    off_t off = 4;
    char comment[BUF_SIZE];

    int fd = open(filename, O_RDONLY);
    if (fd == -1)
        err_sys("Ошибка открытия файла: %s", filename);    

    off_t eof = lseek(fd, EOCDR_BASE_SZ, SEEK_END);
    
    while(eof > 0) {
        read(fd, &sig, sizeof(uint32_t));
        if (sig == EOCDR_SIG) {
            read(fd, in_ecodr,  sizeof(in_ecodr_t));
            cfh_count = in_ecodr->cfh_count;
            if (in_ecodr->comment_len > 0) {
                read(fd, &comment, sizeof(char) * in_ecodr->comment_len);
                comment[in_ecodr->comment_len] = '\0';
                add_comment(ecodr, comment);
            }
            break;
        }
        else {
            eof = lseek(fd, -(off + 1), SEEK_CUR);
        }
    }
    close(fd);
    return cfh_count;    
}



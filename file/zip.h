#ifndef _ZIP_H_
#define _ZIP_H_

#include <stdint-gcc.h>

#define CFH_SIG   0x02014b50   //Center catalog file header
#define LFH_SIG   0x04034b50    
#define EOCDR_SIG 0x06054b50
#define BUF_SIZE 4096

#define SECONDS(a) (((a) & (0x1F)) * 2)
#define MINUTES(a) (((a) & (0x3F)) >> 5)
#define HOURS(a) ((a) >> 11)

#define DAY(a) ((a) & (0x1F))
#define MONTH(a) ((((a) & (0xF)) >> 5) + 1)
#define YEAR(a) (((a) >> 9) + 1980)



typedef struct __attribute__((__packed__)) {
    //4
    uint16_t version;
    uint16_t version_needed;
    uint16_t flags;
    uint16_t compression;  //метод сжатия
    uint16_t modification_time;
    uint16_t modification_date; 
    uint32_t crc;
    uint32_t compressed_size;
    uint32_t uncompressed_size;
    uint16_t filename_len;
    uint16_t extrafield_len;
    uint16_t filecomm_len;
    uint16_t disk_start;
    uint16_t internal_attr;
    uint32_t external_attr;
    uint32_t offset_local_header;
} cfh_short_t;

typedef struct {
    cfh_short_t cfh;
    char* filename;
    char* filecomment;
    uint8_t* extra;
} cfh_t;

void print_file_info(cfh_short_t);
int zip_contains(char*);
int get_str_time(char*, uint16_t);
int get_str_date(char*, uint16_t);
int zip_preview(char*);


#endif
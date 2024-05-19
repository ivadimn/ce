#include "common.h"
#include "file_info.h"
#include "vstr.h"

int main(int argc, char** argv) {
	//vstr_t* str = vstr_dup("GET /admin/login/?next=/admin/ HTTP/2.0");
	//vstr_t* str1 = vstr_dup("GET /%D0%B4%D0%B0-%D0%B0%D0%BB%D1%91-%D0%B4%D0%B0-%D0%B4%D0%B0-%D0%BD%D1%83-%D0%BA%D0%B0%D0%BA-%D1%82%D0%B0%D0%BC-%D1%81-%D0%B4%D0%B5%D0%BD%D1%8C%D0%B3%D0%B0%D0%BC%D0%B8/ HTTP/2.0");
	//vstr_t* url = NULL;

	if(argc < 2) {
		err_quit("Необходимо ввести имя файла");
	}
	
	//long start = vstr_in(str, '/');
	//long end = vstr_instr(str, " HTTP");
	

	//printf("start = %ld, end = %ld\n", start, end);
	//url = vstr_substr(str, start, end);
	//printf("len = %ld\n", url->length);
	
	//vstr_print(url, stdout);

	//decode(url->data, url->length);
	
	handle_file((unsigned char*)argv[1]);
	//vstr_free(str);
	//vstr_free(str1);
	//vstr_free(url);		
	
	return EXIT_SUCCESS;

}
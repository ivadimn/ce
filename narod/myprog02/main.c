#include <stdio.h>

int main() 
{
    printf("String ");
    printf("%s\n", "\"Hellow World!!!\"");
    printf("Char ");
    printf("%c\n", 131);
    printf("Signed integer: ");
	printf("%d\r\n",-1294967296);
    printf("Unigned integer: ");
	printf("%u\r\n",3000000000);
    printf("Signed short: ");
	printf("%hd\r\n",40000);
	printf("Unsigned short: ");
	printf("%hu\r\n",40000);
    printf("Signed long: ");
	printf("%lli\r\n",18446744073709551615);
	printf("Unsigned long: ");
	printf("%llu\r\n",18446744073709551615);
    printf("Float: ");
	printf("%f\r\n",12.456f);
	printf("Double: ");
	printf("%lf\r\n",12345.67891);
    printf("Hex: ");
	printf("0x%llX\r\n",3000000000);
    return 0;
}
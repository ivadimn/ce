/*
	Author: Ivanov Ivan
	Date: 2019.04.05
	Theme: Using comments in the C language
*/

#include <stdio.h>

int main() 
{
	
	//Output a string to the console
	printf("Длина: ");
	printf("%d\n",sizeof(long));
	printf("%d\n",sizeof(unsigned long));

	//Output character to console
	printf("Char: ");
	printf("%c, длина: %ld\r\n",131, sizeof(char));
	//Output a signed integer to the console
	printf("Signed integer: ");
	printf("%ld, длина: %ld\r\n",3000000000, sizeof(int));
	//Output a unsigned integer to the console
	printf("Unigned integer: ");
	printf("%lu, длина: %ld\r\n",3000000000, sizeof(unsigned int));
	//Output a signed short integer to the console
	printf("Signed short: ");
	printf("%hd, длина: %ld\r\n",40000, sizeof(short));
	
	//Output a unsigned short integer to the console
	printf("Unsigned short: ");
	printf("%hu, длина: %ld\r\n",40000, sizeof(unsigned short));
	
	//Output a signed long integer to the console
	printf("Signed long: ");
	printf("%i, длина: %lu\r\n",18446744073709551615, sizeof(long));
	//Output a unsigned long integer to the console
	printf("Unsigned long: ");
	printf("%llu, длина: %lu\r\n",18446744073709551615, sizeof(unsigned long));
	//Outputting a floating-point number to the console
	printf("Float: ");
	printf("%f, длина: %ld\r\n",12.456f, sizeof(float));
	//Outputting a 64-bit floating-point number to the console
	printf("Double: ");
	printf("%lf, длина: %ld\r\n",12345.67891, sizeof(double));
	//Hex output to console
	printf("Hex: ");
	printf("0x%lX\r\n",3000000000);
     return 0;
}
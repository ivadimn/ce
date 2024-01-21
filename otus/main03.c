#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

/*
int main()
{
setlocale(0, "rus");


//размер типа
#if 0
    printf("размер char = %zd \n", sizeof(char));
    printf("размер short = %zd \n", sizeof(short));
    printf("размер int = %zd \n", sizeof(int));
    printf("размер long = %zd \n", sizeof(long));
    printf("размер long long = %zd \n", sizeof(long long));
    printf("размер float = %zd \n", sizeof(float));
    printf("размер double = %zd \n", sizeof(double));
#endif

#if 0
unsigned int un = 3000000000;   // система с 32-битным типом int
short end = 200;                // и 16-битным типом short
long big = 65537;
long long veribig = 12345678908642;
printf("un = %u, но не %d\n", un, un);
printf("end = %hd и %d\n", end, end);
printf("big = %ld, но не %hd\n", big, big);
printf("veribig = %lld, но не %ld\n", veribig, veribig);
#endif


// переполнение
#if 0
    int8_t i8 = 127;
        printf("x = %d \n", i8);
    i8++;
        printf("x + 1 = %d \n", i8);
    i8 *=2;
        printf("x * 2 = %d \n", i8);
#endif

// неявное приведение типов
#if 0
    int si = -100;
    unsigned int uni = 10U;
    long long int lli = si + uni;
        printf("si + uni = %lld" , lli);
#endif

#if 0
    char ch = 128;
    int i = ch * 10;
        printf("i = %d", i);
#endif

// явное приведение типов
#if 0
    int16_t si_8 =1000;
        printf("si_8 = %d \n", si_8);
        printf("si_8 = %" PRId8, si_8);
#endif



//вещественные числа
#if 0
    float flt = 45000.0;
    double dbl = 5.15e9;
        printf("float десятичное представление - %f \n", flt);
        printf("float экспоненциальное представление - %e \n", flt);
        printf("float шестнадцатеричное представление - %a \n", flt);
        printf("double %f == %e \n", dbl, dbl);
#endif

//ошибка округления
#if 1
    double a, b;
    a = 2.e10 + 1;
    b = a - 2.e10;
        printf("%25.15f\n", b);
#endif

    return 0;
}
*/


#pragma pack(push, 1)
typedef struct
{
  unsigned char x;
  unsigned short y;
  unsigned int z;
} my_arg_t;
#pragma pack(pop)

int main()  {
    printf("sizeof data is %d\n", sizeof(my_arg_t));
    my_arg_t my_arg;
    my_arg.z = 0x11223344;
    printf("z = 0x%08X\n", my_arg.z);
    my_arg.y = 0x5566;
    printf("y = 0x%04X\n", my_arg.y);
    my_arg.x = 0x77;
    printf("x = 0x%02X\n", my_arg.x);
    return 0;
}



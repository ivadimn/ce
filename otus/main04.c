#include <stdio.h>
#include <stdlib.h>

/*int main()
{
    int mas[5] = {4, 6};
    char str[] = "Это массив!!!";
    char *p = str;
    setlocale(0, "rus");

#if 1
    printf("%d\n", mas[0]);
    printf("%d\n", *mas );

    printf("\n%d\n", &mas);
    printf("%p\n", mas );

    printf("Размер массива %d\n", sizeof(str));
    printf("Размер указателя на массива %d\n", sizeof(p));
#endif

#if 0
    for(int i = 0; i < 5; ++i)
        printf("%d ", mas[i]);
#endif

#if 0
    if (&str == p)
        printf("Yes");
    else
        printf("No");
#endif

    return 0;
}
*/


// Раздел 6.2.5.22 Стандарта: An array type of unknown size is an incomplete type. It is completed,
//  for an identifier of that type, by specifying the size in a later declaration.
#if 0
double average(int arr[], int size)
{
    double sum = 0;
    //printf("%d\n", sizeof(arr));
    for(int i = 0; i < size; ++i)
        sum += arr[i];
    return sum / size;
}

int main()
{
    int balance[5] = {1000, 2, 3, 17, 50};
    printf("Average value is: %f", average(balance, 5));
    return 0;
}
#endif

//Массивы переменной длины (VLA)
//C99+
#if 0
void read_and_process(int n)
{
    float vals[n];
    for (int i = 0; i < n; ++i)
    {   vals[i] = i;
        printf("%.2f ", vals[i]);
    }
}

int main()
{
    int count;
    do {
        printf("\nsize array - ");
        scanf("%d", &count);
        read_and_process(count);
    } while (count);

    return 0;
}
#endif


#if 0
int main()
{
    char *str = "very very"
                "very very"
                "long string";

    char str2[] =   "very very \
            very very \
            long string";

            printf("%s \n", str);
    printf("%s \n", str2);

    return 0;
}
#endif


//Функции с переменным числом аргументов
#if 0
#include <stdarg.h>
double average(int count, ...)
{
    va_list ap;
    int j;
    double sum = 0;
    va_start(ap, count);
    for (j = 0; j < count; j++)
    {
        sum += va_arg(ap, double);
    }
    va_end(ap);
    return sum / count;
}
int main()
{
    printf("%f; %f\n",
           average(2, 1.0, 2.0),
           average(3, 1.0, 2.0, 3.0));
}
#endif


#if 1
#include <sys/stat.h>
#include <time.h>
#include <errno.h>

int main ()
{
    FILE *fp;
    struct stat buf;
    int fh, result;
    setlocale(0, "rus");

    fp = fopen("file.txt","w+");  // if ((fp=fopen("test", "rb")) == NULL)
    result=fstat(fileno (fp), &buf);
    if (result !=0)
        printf("Плохой дескриптор файла\n");
    else
    {
        printf("Размер файла: %ld\n", buf.st_size);
        printf("Номер устройства: %d\n", buf.st_dev);
        printf("Время модификации: %s",
               ctime(&buf.st_atime));
    }

    fputs("This is course Otus", fp);
    fseek( fp, 7, SEEK_SET );
    fputs(" C Programming Language", fp);
    fclose(fp);

    return(0);
}
#endif

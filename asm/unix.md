# История, философия и основы работы в UNIX

## POSIX

![](life-with-posix.jpg)

IEEE Standard for Information Technology-Portable Operating System Interface Base Specifications, Issue 7, IEEE Std 1003.1-2017

POSIX defines a standard operating system interface and environment, including a command interpreter (or "shell"), and common utility programs to support **applications portability at the source code level**.

<https://pubs.opengroup.org/onlinepubs/9699919799>

<https://pubs.opengroup.org/onlinepubs/9699919799/download/index.html>

[Single UNIX Specification](https://unix.org/what_is_unix/single_unix_specification.html), [Single UNIX Specification - Википедия](https://ru.wikipedia.org/wiki/Single_UNIX_Specification)

[standards(7)](https://man7.org/linux/man-pages/man7/standards.7.html)

POSIX-certified:

* AIX
* HP-UX
* IRIX
* UnixWare
* Solaris
* macOS 10.5

Mostly POSIX-compliant:

* BeOS & Haiku
* Minix
* Xenix
* *BSD
* [GNU](https://ru.wikipedia.org/wiki/GNU)/Linux (`POSIXLY_CORRECT`; [Linux Standard Base](https://refspecs.linuxfoundation.org/lsb.shtml), ISO/IEC 23360-1:2006)

POSIX for Windows:

* MSVCRT & Winsock, the support remains largely incomplete and not fully interoperable.
* Cygwin
* MinGW

Состав POSIX:

* Base definitions
* Shell and utilities
* [System interfaces](https://pubs.opengroup.org/onlinepubs/9699919799/idx/functions.html)
* Rationale

Важнейшие понятия:

* пользователь
* файл
* процесс
* терминал
* хост
* узел сети
* время
* языково-культурная среда
* системный вызов

[C POSIX Library](https://pubs.opengroup.org/onlinepubs/9699919799/idx/head.html)

<https://gnu.org/software/libc/manual/html_node/Feature-Test-Macros.html>

[Glibc feature test macros](https://lwn.net/Articles/590381)

```c
#define _POSIX_C_SOURCE 200112L
```

### Системные вызовы

```c
    const char msg[] = "Hello World";
    write(1, msg, sizeof(msg));
```

---

```asm
section .data
    msg: db "Hello World", 0x0
    len: equ $-msg

section .text
    mov rax, 1    ; use the `write` syscall
    mov rdi, 1    ; write to stdout
    mov rsi, msg  ; use string "Hello World"
    mov rdx, len  ; write 11 characters
    syscall       ; make system call
```

[Direct Operating System Access via Syscalls](https://www.cs.uaf.edu/2017/fall/cs301/lecture/11_17_syscall.html)

[x86 Assembly/Interfacing with Linux - Wikibooks](https://en.wikibooks.org/wiki/X86_Assembly/Interfacing_with_Linux)

[Searchable Linux Syscall Table for x86 and x86_64](https://filippo.io/linux-syscall-table)

## Философия UNIX

<https://ru.wikipedia.org/wiki/Философия_Unix>

[The Unix Philosophy: A Brief Introduction](http://linfo.org/unix_philosophy.html), [перевод](https://habr.com/ru/company/otus/blog/579680)

[Deconstructing the "Unix philosophy"](https://tedinski.com/2018/05/08/case-study-unix-philosophy.html), [перевод](https://habr.com/ru/company/timeweb/blog/690844)

Design programs to do only a single thing, but to do it well, and to work together well with other programs.

### Введение в UNIX

* [Unix shell: абсолютно первые шаги](https://habr.com/ru/post/267825)
* [Command line crash course](https://developer.mozilla.org/en-US/docs/Learn/Tools_and_testing/Understanding_client-side_tools/Command_line)
* [Learn Enough Command Line to Be Dangerous](https://learnenough.com/command-line-tutorial)
* [Краткий справочник по «всем-всем» командам Linux](https://habr.com/ru/company/lanit/blog/683638)
* [Unix Toolbox](http://subnets.ru/wrapper.php?p=33), [перевод](https://vds-admin.ru/unix-toolbox)
* [Шпаргалка по работе в Vim](https://eax.me/vim-commands)
* [Byte of Vim](https://github.com/swaroopch/byte-of-vim)

## Интерфейс командной строки

[argv silliness](https://ryiron.wordpress.com/2013/12/16/argv-silliness), [CVE-2021-4034](https://seclists.org/oss-sec/2022/q1/80)

[Command Line Interface Guidelines](https://clig.dev)

### getopt

[getopt(3)](https://man7.org/linux/man-pages/man3/getopt.3.html)

```c
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

int main (int argc, char **argv) {
    int c;
    int digit_optind = 0;

    while (1) {
        int this_option_optind = optind ? optind : 1;
        int option_index = 0;
        static struct option long_options[] = {
            {"add", 1, 0, 0},
            {"append", 0, 0, 0},
            {"delete", 1, 0, 0},
            {"verbose", 0, 0, 0},
            {"create", 1, 0, 'c'},
            {"file", 1, 0, 0},
            {0, 0, 0, 0}
        };

        c = getopt_long (argc, argv, "abc:d:012",
                 long_options, &option_index);
        if (c == -1)
            break;

        switch (c) {
        case 0:
            printf ("параметр %s", long_options[option_index].name);
            if (optarg)
                printf (" с аргументом %s", optarg);
            printf ("\n");
            break;

        case '0':
        case '1':
        case '2':
            if (digit_optind != 0 && digit_optind != this_option_optind)
              printf ("цифры используются в двух разных элементах argv.\n");
            digit_optind = this_option_optind;
            printf ("параметр  %c\n", c);
            break;

        case 'a':
            printf ("параметр a\n");
            break;

        case 'b':
            printf ("параметр b\n");
            break;

        case 'c':
            printf ("параметр c со значением `%s'\n", optarg);
            break;

        case 'd':
            printf ("параметр d со значением `%s'\n", optarg);
            break;

        case '?':
            break;

        default:
            printf ("?? getopt возвратило код символа 0%o ??\n", c);
        }
    }

    if (optind < argc) {
        printf ("элементы ARGV, не параметры: ");
        while (optind < argc)
            printf ("%s ", argv[optind++]);
        printf ("\n");
    }

    exit (0);
}
```

### popt

<https://github.com/rpm-software-management/popt>

```c
#include <stdio.h>
#include <stdlib.h>
#include <popt.h>

void usage(poptContext optCon, int exitcode, char *error, char *addl) {
    poptPrintUsage(optCon, stderr, 0);
    if (error) fprintf(stderr, "%s: %s", error, addl);
    exit(exitcode);
}

int main(int argc, char *argv[]) {
   char    c;            /* used for argument parsing */
   int     i = 0;        /* used for tracking options */
   char    *portname;
   int     speed = 0;    /* used in argument parsing to set speed */
   int     raw = 0;      /* raw mode? */
   int     j;
   char    buf[BUFSIZ+1];
   poptContext optCon;   /* context for parsing command-line options */

   struct poptOption optionsTable[] = {
      { "bps", 'b', POPT_ARG_INT, &speed, 0,
                          "signaling rate in bits-per-second", "BPS" },
      { "crnl", 'c', 0, 0, 'c',
                          "expand cr characters to cr/lf sequences", NULL },
      { "hwflow", 'h', 0, 0, 'h',
                          "use hardware (RTS/CTS) flow control", NULL },
      { "noflow", 'n', 0, 0, 'n',
                          "use no flow control", NULL },
      { "raw", 'r', 0, &raw, 0,
                          "don't perform any character conversions", NULL },
      { "swflow", 's', 0, 0, 's',
                          "use software (XON/XOF) flow control", NULL } ,
      POPT_AUTOHELP
      { NULL, 0, 0, 0, 0,  NULL, NULL }
    };

   optCon = poptGetContext(NULL, argc, argv, optionsTable, 0);
   poptSetOtherOptionHelp(optCon, "[OPTIONS]* <port>");

   if (argc < 2) {
      poptPrintUsage(optCon, stderr, 0);
      exit(1);
   }

   /* Now do options processing, get portname */
   while ((c = poptGetNextOpt(optCon)) >= 0) {
      switch (c) {
       case 'c':
          buf[i++] = 'c';
          break;
       case 'h':
          buf[i++] = 'h';
          break;
       case 's':
          buf[i++] = 's';
          break;
       case 'n':
          buf[i++] = 'n';
          break;
      }
   }
   portname = poptGetArg(optCon);
   if((portname == NULL) || !(poptPeekArg(optCon) == NULL))
      usage(optCon, 1, "Specify a single port", ".e.g., /dev/cua0");

   if (c < -1) {
      /* an error occurred during option processing */
      fprintf(stderr, "%s: %s\n",
              poptBadOption(optCon, POPT_BADOPTION_NOALIAS),
              poptStrerror(c));
      return 1;
   }

   /* Print out options, portname chosen */
   printf("Options  chosen: ");
   for(j = 0; j < i ; j++)
      printf("-%c ", buf[j]);
   if(raw) printf("-r ");
   if(speed) printf("-b %d ", speed);
   printf("\nPortname chosen: %s\n", portname);

   poptFreeContext(optCon);
   exit(0);
}
```

---

```
Usage: a.out [OPTIONS]* <port>
  -b, --bps=BPS     signaling rate in bits-per-second
  -c, --crnl        expand cr characters to cr/lf sequences
  -h, --hwflow      use hardware (RTS/CTS) flow control
  -n, --noflow      use no flow control
  -r, --raw         don't perform any character conversions
  -s, --swflow      use software (XON/XOF) flow control

Help options:
  -?, --help        Show this help message
      --usage       Display brief usage message
```

### docopt

<https://github.com/docopt/docopt.c>

```
Naval Fate.

Usage:
  naval_fate ship create <name>...
  naval_fate ship <name> move <x> <y> [--speed=<kn>]
  naval_fate ship shoot <x> <y>
  naval_fate mine (set|remove) <x> <y> [--moored|--drifting]
  naval_fate --help
  naval_fate --version

Options:
  -h --help     Show this screen.
  --version     Show version.
  --speed=<kn>  Speed in knots [default: 10].
  --moored      Moored (anchored) mine.
  --drifting    Drifting mine.
```

---

```sh
python -m docopt_c -o docopt.c example.docopt
```

---

```c
#include "docopt.h"

int main(int argc, char *argv[])
{
    struct DocoptArgs args = docopt(argc, argv,
        /* help */ 1, /* version */ "2.0rc2");

    puts("Commands");
    printf("\tmine == %s\n", args.mine ? "true" : "false");

    puts("Arguments");
    printf("\tx == %s\n", args.x);

    puts("Flags");
    printf("\t--drifting == %s\n", args.drifting ? "true" : "false");

    return EXIT_SUCCESS;
}
```

### gengetopt

[GNU Gengetopt](https://gnu.org/software/gengetopt/gengetopt.html)

```
# cmdline.ggo
package "server"
purpose "Highload Cup entry"
version "2.0"

option "port" p
  "Port to listen on"
  int default="8080"
  typestr="PORT"
  optional

option "verbose" v
  "Be verbose"
  flag off

option "data" d
  "Location of data.zip file"
  string default="/tmp/data/data.zip"

option "threads" t
  "Number of threads to launch"
  int default="4"
  optional
```

---

```
$ ./server --help
server 2.0

Highload Cup entry

Usage: server [OPTIONS]...

  -h, --help         Print help and exit
  -V, --version      Print version and exit
  -p, --port=PORT    Port to listen on  (default=`8080')
  -v, --verbose      Be verbose  (default=off)
  -d, --data=STRING  Location of data.zip file  (default=`/tmp/data/data.zip')
  -t, --threads=INT  Number of threads to launch  (default=`4')
```

## Перенаправление ввода-вывода

[Pipe: How the System Call That Ties Unix Together Came About](https://thenewstack.io/pipe-how-the-system-call-that-ties-unix-together-came-about)

```sh
ls / | grep tmp
```

![](pipe-redirection.png)

## Файловая система

![](fs.png)

[chroot(2)](https://man7.org/linux/man-pages/man2/chroot.2.html)

![](files.jpg)

![](permissions.png)

[chmod calculator](https://chmod-calculator.com)

[chmod(2)](https://man7.org/linux/man-pages/man2/chmod.2.html)

[stat(2)](https://man7.org/linux/man-pages/man2/lstat.2.html)

## Инструменты сборки

### Make

[Владимир Игнатов, Эффективное использование GNU Make](http://opennet.ru/docs/RUS/gnumake)

```make
# Makefile

all: hello

hello: main.c libmy.a
	$(CC) $(CFLAGS) -Wall -Wextra -pedantic -std=c11 `pkg-config --cflags --libs libcurl` $^ -o $@

libmy.a: my.o
	$(AR) rcs $@ $^

my.o: my.c
	$(CC) -c $(CFLAGS) -Wall -Wextra -Wpedantic -std=c11 $^ -o $@

clean:
	$(RM) hello libmy.a core *.o

.PHONY: all clean
```

[Automatic Variables](https://gnu.org/software/make/manual/html_node/Automatic-Variables.html)

[Built-in Rules](https://gnu.org/software/make/manual/html_node/Catalogue-of-Rules.html)

BSD make vs GNU make: GNUMakefile

```make
GNUMAKE?=gmake

all:
	${GNUMAKE} $@

.DEFAULT:
	${GNUMAKE} $@

.PHONY: all
```

[A Tutorial on Portable Makefiles](https://nullprogram.com/blog/2017/08/20)

#### Makefile best practices

* Makefiles should remove all content that it will generate.
* Dependency builds - A second invocation of make within a sandbox should always be a NOP.
* Hardcoded values - avoid them like the plague.
* Always include dependencies when creating a target.

[Makefiles - Best practices and suggestions](http://udn.realityripple.com/docs/Mozilla/Developer_guide/Build_Instructions/How_Mozilla_s_build_system_works/Makefiles_-_Best_practices_and_suggestions)

[Makefiles, Best Practices](https://web.archive.org/web/20230226191918/https://danyspin97.org/blog/makefiles-best-practices)

[Your Makefiles are wrong](https://tech.davis-hansson.com/p/make)

[Reproducible Builds](https://reproducible-builds.org)

### pkg-config

pkg-config was originally designed for Linux, but it is now also available for BSD, Microsoft Windows, macOS, and Solaris.

[Guide to pkg-config](https://people.freedesktop.org/~dbn/pkg-config-guide.html)

```
# /usr/lib/x86_64-linux-gnu/pkgconfig/libpng.pc

prefix=/usr/local
exec_prefix=${prefix}
libdir=${exec_prefix}/lib
includedir=${exec_prefix}/include

Name: libpng
Description: Loads and saves PNG files
Version: 1.2.8
Libs: -L${libdir} -lpng12 -lz
Cflags: -I${includedir}/libpng12
```

---

```
$ pkg-config --list-all | grep libpng
libpng                         libpng - Loads and saves PNG files

$ pkg-config --cflags libpng
-I/usr/include/libpng12

$ pkg-config --libs libpng
-lpng12

$ pkg-config --cflags --libs libpng
-I/usr/include/libpng12 -lpng12
```

### autotools

![](autotools.png)

[Autotools Mythbuster](https://autotools.info)

[Основы сборки проектов при помощи Autotools](https://eax.me/autotools)

![](autotools-popularity.png)
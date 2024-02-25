# Стандарт POSIX и программирование под UNIX

[Unix as IDE](https://blog.sanctum.geek.nz/series/unix-as-ide), [перевод](https://habr.com/ru/post/150930)

## Современные системы сборки

### CMake

![](cmake_logo.png)

<https://cmake.org>

```sh
cmake -G
cmake -D
cmake-gui
```

---

```cmake
# CMakeLists.txt

cmake_minimum_required(VERSION 3.10)

project(Hello LANGUAGES C)

find_package(CURL REQUIRED)

include_directories(${CURL_INCLUDE_DIRS})

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

add_compile_options(-Wall -Wextra -Wpedantic)

add_library(my STATIC lib.c)

add_executable(hello main.c)

target_link_libraries(hello PUBLIC my ${CURL_LIBRARIES})
```

[CMake tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)

[Дубров Д. В. Система построения проектов CMake](https://litres.ru/book/denis-vladimirovich-dubrov/sistema-postroeniya-proektov-cmake-36059397)

[CMake cookbook](https://ozon.ru/product/cmake-cookbook-157693062)

### Ninja

<https://ninja-build.org>

> Where other build systems are high-level languages Ninja aims to be an assembler.
> Ninja build files are human-readable but not especially convenient to write by hand.
> Ninja is used to build Google Chrome, parts of Android, LLVM, and can be used in many other projects due to CMake's Ninja backend.

```
cflags = -Wall -Wextra -Wpedantic

rule cc
  command = gcc $cflags -c $in -o $out

build main.o: cc main.c
```

[Ninja is enough build system](https://bernsteinbear.com/blog/ninja-is-enough)

### Meson

![](meson_logo.png)

<https://mesonbuild.com>

```
project('Hello, 'c')
executable('hello, 'main.c')
```

[The Absolute Beginner's Guide to Installing and Using Meson](https://mesonbuild.com/SimpleStart.html)

### Bazel

<https://bazel.build>

[Bazel Tutorial: Build a C++ Project](https://bazel.build/start/cpp)

## Diff & Patch

[diff(1)](https://man7.org/linux/man-pages/man1/diff.1.html)

[patch(1)](https://man7.org/linux/man-pages/man1/patch.1.html)

Оригинальная версия программы была написана будущим автором языка Perl Larry Wall в 1985.

```
$ diff -u старый_файл новый_файл > разница.diff

$ patch < разница.diff

$ diff -ruN старый_каталог новый_каталог > разница.diff

$ patch -p0 < разница.diff
```

---

```patch
--- a/path/to/file	2021-01-26 22:55:55.288371691 +0300
+++ b/path/to/file	2021-01-26 22:58:31.790414616 +0300
@@ -1,5 +1,8 @@
+#include <stdio.h>
+
 int
-main(void)
+main(int argc, char** argv)
 {
+	printf("%s: Example `diff` usage;\n", __FILE__);
 	return 0;
 }
```

## Отладка: GDB

![](gdb_logo.png)

[GDB: The GNU Project Debugger](https://gnu.org/software/gdb)

Наиболее важные команды:

* [(h)elp](https://sourceware.org/gdb/current/onlinedocs/gdb.html/Help.html#index-help)
* [file](https://sourceware.org/gdb/current/onlinedocs/gdb.html/Files.html#index-file)
* [(r)un](https://sourceware.org/gdb/current/onlinedocs/gdb.html/Starting.html#index-run)
* [(l)ist](https://sourceware.org/gdb/current/onlinedocs/gdb.html/List.html#index-list)
* [(dis)assemble](https://sourceware.org/gdb/current/onlinedocs/gdb.html/Machine-Code.html#index-disassemble)
* [(b)reak](https://sourceware.org/gdb/current/onlinedocs/gdb.html/Set-Breaks.html#index-break)
* [(c)ontinue, (s)tep, (n)ext](https://sourceware.org/gdb/current/onlinedocs/gdb.html/Continuing-and-Stepping.html)
* [(fin)ish](https://sourceware.org/gdb/current/onlinedocs/gdb.html/Continuing-and-Stepping.html#index-finish)
* [(p)rint](https://sourceware.org/gdb/current/onlinedocs/gdb.html/Data.html#index-print)
* [x](https://sourceware.org/gdb/current/onlinedocs/gdb.html/Memory.html#index-x-_0028examine-memory_0029)
* [backtrace (bt)](https://sourceware.org/gdb/current/onlinedocs/gdb.html/Backtrace.html#index-backtrace)
* [(f)rame](https://sourceware.org/gdb/current/onlinedocs/gdb.html/Selection.html#index-frame_002c-selecting)

[Памятка по отладке при помощи GDB](https://eax.me/gdb)

[GDB dashboard](https://github.com/cyrus-and/gdb-dashboard)

[Python Exploit Development Assistance for GDB](https://github.com/longld/peda)

## Valgrind

![](valgrind_logo.png)

[Что такое valgrind и зачем он нужен](http://alexott.net/ru/writings/prog-checking/Valgrind.html)

[Поиск ошибок работы с памятью в C/C++ при помощи Valgrind](https://eax.me/valgrind)

* `memcheck`: основной модуль, обеспечивающий обнаружение утечек памяти, и прочих ошибок, связанных с неправильной работой с областями памяти — чтением или записью за пределами выделенных регионов и т.п.
* `cachegrind`: анализирует выполнение кода, собирая данные о (не)попаданиях в кэш, и точках перехода (когда процессор неправильно предсказывает ветвление). Эта статистика собирается для всей программы, отдельных функций и строк кода.
* `callgrind`: анализирует вызовы функций, используя примерно ту же методику, что и модуль cachegrind. Позволяет построить дерево вызовов функций, и соответственно, проанализировать узкие места в работе программы.
* `massif`: позволяет проанализировать выделение памяти различными частями программы.
* `helgrind`: анализирует выполняемый код на наличие различных ошибок синхронизации, при использовании многопоточного кода, использующего POSIX Threads.

```
$ valgrind --leak-check=full --show-leak-kinds=all -- ./a.out args
```

* *definitely lost*: heap-allocated memory that was never freed to which the program no longer has a pointer. Valgrind knows that you once had the pointer, but have since lost track of it. This memory is definitely orphaned.
* *indirectly lost*: heap-allocated memory that was never freed to which the only pointers to it also are lost. For example, if you orphan a linked list, the first node would be definitely lost, the subsequent nodes would be indirectly lost.
* *possibly lost*: heap-allocated memory that was never freed to which valgrind cannot be sure whether there is a pointer or not.
* *still reachable*: heap-allocated memory that was never freed to which the program still has a pointer at exit.

```
==15788== 16 bytes in 1 blocks are definitely lost in loss record 10 of 37
==15788==    at 0x4C2DB8F: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==15788==    by 0x401844: get (get.c:25)
==15788==    by 0x4013DE: _get_weather_by_woeid (main.c:85)
==15788==    by 0x40169A: main (main.c:148)
==15788==
==15788== 25,662 (48 direct, 25,614 indirect) bytes in 1 blocks are definitely lost in loss record 37 of 37
==15788==    at 0x4C2DB8F: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==15788==    by 0x4E3E947: json_object_new (json_object.c:353)
==15788==    by 0x4E3F019: json_object_new_object (json_object.c:558)
==15788==    by 0x4E431E7: json_tokener_parse_ex (json_tokener.c:349)
==15788==    by 0x4E42CF9: json_tokener_parse_verbose (json_tokener.c:200)
==15788==    by 0x4E42C9C: json_tokener_parse (json_tokener.c:188)
==15788==    by 0x4013FA: _get_weather_by_woeid (main.c:89)
==15788==    by 0x40169A: main (main.c:148)
==15788==
==15788== LEAK SUMMARY:
==15788==    definitely lost: 135 bytes in 5 blocks
==15788==    indirectly lost: 26,719 bytes in 382 blocks
==15788==      possibly lost: 0 bytes in 0 blocks
==15788==    still reachable: 0 bytes in 0 blocks
==15788==         suppressed: 0 bytes in 0 blocks
==15788==
==15788== For counts of detected and suppressed errors, rerun with: -v
==15788== Use --track-origins=yes to see where uninitialised values come from
==15788== ERROR SUMMARY: 19 errors from 15 contexts (suppressed: 0 from 0)
```

Визуализация результатов профилировки `callgrind`: [KCacheGrind](http://kcachegrind.sourceforge.net/html/Home.html).

[Memory error checking in C and C++: Comparing Sanitizers and Valgrind](https://developers.redhat.com/blog/2021/05/05/memory-error-checking-in-c-and-c-comparing-sanitizers-and-valgrind)

## Профилирование

[Профилирование кода на C/C++ в Linux и FreeBSD](https://eax.me/c-cpp-profiling/)

* Valgrind
* [strace(1)](https://man7.org/linux/man-pages/man1/strace.1.html) / [ltrace(1)](https://man7.org/linux/man-pages/man1/ltrace.1.html)
* [bpftrace](https://bpftrace.org), [Основы трассировки с помощью bpftrace](https://eax.me/bpftrace)
* [GNU gprof](http://sourceware.org/binutils/docs/gprof) + KCacheGrind
* [Google Performance Tools](https://github.com/gperftools/gperftools)
* [perf](http://perf.wiki.kernel.org)
  * [flame graphs](http://brendangregg.com/perf.html#FlameGraphs)
  * [Linux perf Profiler UIs](https://markhansen.co.nz/profiler-uis)
  * [Профилирование и трейсинг с perf](https://habr.com/ru/company/first/blog/442738)

[A Look at Profiling: FreeBSD Sort](https://freebsdfoundation.org/blog/a-look-at-profiling-freebsd-sort), [перевод](https://habr.com/ru/company/otus/blog/667286)

[Make your QEMU 10 times faster with this one weird trick](https://determinate.systems/posts/qemu-fix)

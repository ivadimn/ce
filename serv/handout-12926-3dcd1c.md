# Асинхронные сетевые интерфейсы

[Основные методы ввода-вывода](https://tproger.ru/translations/diversity-of-input-output-io)

[7 минут]::
## Неблокирующие операции на сокетах

По умолчанию блокирующие операции сокетов:

* чтения (`read`, `readv`, `recv`, `recvfrom`, `recvmsg`)
* записи (`write`, `writev`, `send`, `sendto`, `sendmsg`)
* приём входящих соединений (`accept`)
* инициирование исходящих соединений (`connect`)

Установка неблокирующего режима сокета через [fcntl(2)](https://linux.die.net/man/2/fcntl):

```c
const int flags = fcntl(fd, F_GETFL, 0);
fcntl(fd, F_SETFL, flags | O_NONBLOCK);
```

После выставления неблокирующего режима любая блокирующая операция будет вовзвращать ошибку `EAGAIN` (`EWOULDBLOCK`) в `errno`.

![](nonblocking.png)

[4 минуты]::
## Мультиплексирование ввода-вывода

![](multiplexing.png)

`SIGPOLL` / `SIGIO` - устаревший механизм.

[30 минут]::
### select

[select(2)](https://linux.die.net/man/2/select)

```c
#include <sys/select.h>

int select(int nfds, fd_set* readfds, fd_set* writefds,
    fd_set* exceptfds, struct timeval* timeout);

int FD_ISSET(int fd, fd_set* fdset);
void FD_CLR(int fd, fd_set* fdset);
void FD_SET(int fd, fd_set* fdset);
void FD_ZERO(fd_set* fdset);
```

![](fd_set.png)

> File descriptors associated with regular files shall always select true for ready to read, ready to write, and error conditions.

[timerfd_create(2)](https://linux.die.net/man/2/timerfd_create)

[Example: Nonblocking I/O and select() - IBM Documentation](https://ibm.com/docs/en/i/7.2?topic=designs-example-nonblocking-io-select)

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>


#define PORT 12345

static char buffer[2048];

bool set_nonblocking(int fd)
{
    const int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0)
    {
        perror("fcntl");
        return false;
    }
    int rc = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    if (rc < 0)
    {
        perror("fcntl");
        return false;
    }
    return true;
}

int main()
{
   int rc;

   int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
   if (listen_fd < 0)
   {
      perror("socket");
      exit(EXIT_FAILURE);
   }

   if (!set_nonblocking(listen_fd))
   {
       close(listen_fd);
       exit(EXIT_FAILURE);
   }

   struct sockaddr_in addr = {0};
   addr.sin_family      = AF_INET;
   addr.sin_addr.s_addr = htonl(INADDR_ANY);
   addr.sin_port        = htons(PORT);
   rc = bind(listen_fd, (struct sockaddr*)&addr, sizeof(addr));
   if (rc < 0)
   {
      perror("bind");
      close(listen_fd);
      exit(EXIT_FAILURE);
   }

   rc = listen(listen_fd, 32);
   if (rc < 0)
   {
      perror("listen");
      close(listen_fd);
      exit(EXIT_FAILURE);
   }

   fd_set master_set, working_set;
   FD_ZERO(&master_set);
   int max_fd = listen_fd;
   FD_SET(listen_fd, &master_set);

   struct timeval timeout;
   timeout.tv_sec  = 3 * 60;
   timeout.tv_usec = 0;

   bool end_server = false;
   while (!end_server)
   {
      memcpy(&working_set, &master_set, sizeof(master_set));

      rc = select(max_fd + 1, &working_set, NULL, NULL, &timeout);
      if (rc < 0)
      {
         perror("select");
         break;
      }
      if (rc == 0)
      {
         printf("Time out, ending program.\n");
         break;
      }

      int desc_ready = rc;
      for (int i = 0; i <= max_fd && desc_ready > 0; i++)
      {
         if (FD_ISSET(i, &working_set))
         {
            desc_ready--;
            if (i == listen_fd)
            {
               int new_fd;
               do
               {
                  new_fd = accept(listen_fd, NULL, NULL);
                  if (new_fd < 0)
                  {
                     if (errno != EWOULDBLOCK)
                     {
                        perror("accept");
                        end_server = true;
                     }
                     break;
                  }

                  set_nonblocking(new_fd);
                  FD_SET(new_fd, &master_set);
                  if (new_fd > max_fd)
                     max_fd = new_fd;
               } while (new_fd != -1);
            }
            else
            {
               bool close_conn = false;
               for(;;)
               {
                  rc = recv(i, buffer, sizeof(buffer), 0);
                  if (rc < 0)
                  {
                     if (errno != EWOULDBLOCK)
                     {
                        perror("recv");
                        close_conn = true;
                     }
                     break;
                  }
                  if (rc == 0)
                  {
                     close_conn = true;
                     break;
                  }

                  int len = rc;
                  rc = send(i, buffer, len, 0);
                  if (rc < 0)
                  {
                     perror("send");
                     close_conn = true;
                     break;
                  }
               }

               if (close_conn)
               {
                  close(i);
                  FD_CLR(i, &master_set);
                  if (i == max_fd)
                  {
                     while (FD_ISSET(max_fd, &master_set) == false)
                        max_fd -= 1;
                  }
               }
            }
         }
      }
   }

   for (int i = 0; i <= max_fd; ++i)
   {
      if (FD_ISSET(i, &master_set))
         close(i);
   }
}
```

[Select is fundamentally broken](https://idea.popcount.org/2017-01-06-select-is-fundamentally-broken)

[6 минут]::
### poll

[The problem with select() vs poll()](https://dev.to/fluffy/the-problem-with-select-vs-poll-2k1h)

[poll(2)](https://linux.die.net/man/2/poll)

```c
#include <poll.h>

struct pollfd {
    int fd;
    short events;
    short revents;
};

int poll(struct pollfd fds[], nfds_t nfds, int timeout);

/* events, revents */
POLLIN;
POLLOUT;

/* revents */
POLLERR;
POLLHUP;
POLLNVAL;
```

`/dev/poll`: [poll(7)](https://www.opennet.ru/man.shtml?topic=poll&category=7&russian=4)

[2 минуты]::
### AIO

[aio(7)](https://linux.die.net/man/7/aio)

В Glibc AIO реализованы поверх pthreads и обычного `pread`: [AIO network sockets and zero-copy under Linux](https://stackoverflow.com/a/2982208/1336774)

[25 минут]::
### epoll

[Async IO on Linux: select, poll and epoll](https://jvns.ca/blog/2017/06/03/async-io-on-linux--select--poll--and-epoll)

[Асинхронный ввод-вывод средствами POSIX](https://ps-group.github.io/os/nonblocking_io_posix)

[Вся правда о linux epoll](https://habr.com/ru/post/416669)

[epoll(7)](https://linux.die.net/man/7/epoll)

![](epoll-struct.png)

[epoll_create(2)](https://linux.die.net/man/2/epoll_create)

```c
#include <sys/epoll.h>

int epoll_create(int size);
int epoll_create1(int flags);

/* flags */
EPOLL_CLOEXEC;
```

![](epoll_create.jpeg)

[epoll_ctl(2)](https://linux.die.net/man/2/epoll_ctl)

```c
#include <sys/epoll.h>

typedef union epoll_data {
    void        *ptr;
    int          fd;
    uint32_t     u32;
    uint64_t     u64;
} epoll_data_t;

struct epoll_event {
    uint32_t     events;
    epoll_data_t data;
};

int epoll_ctl(int epfd, int op, int fd, struct epoll_event* event);

/* op */
EPOLL_CTL_ADD;
EPOLL_CTL_MOD;
EPOLL_CTL_DEL;

/* events */
EPOLLIN;
EPOLLOUT;
EPOLLRDHUP;
EPOLLERR;
EPOLLHUP;
EPOLLET;
EPOLLONESHOT;
```

![](epoll_ctl.jpeg)

> In edge-triggered mode, a call to epoll_wait will return only when a new event is enqueued with the epoll object, while in level-triggered mode, epoll_wait will return as long as the condition holds.

[What is the purpose of epoll's edge triggered option?](https://stackoverflow.com/a/9162805/1336774)

[epoll_wait(2)](https://linux.die.net/man/2/epoll_wait)

```c
#include <sys/epoll.h>

int epoll_wait(int epfd, struct epoll_event* events, int maxevents, int timeout);
```

---


```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <unistd.h>


static char buffer[2048];

int setnonblocking(int sock)
{
 int opts;

 opts = fcntl(sock,F_GETFL);
 if (opts < 0)
 {
  perror("fcntl(F_GETFL)");
  return -1;
 }
 opts = (opts | O_NONBLOCK);
 if (fcntl(sock,F_SETFL,opts) < 0)
 {
  perror("fcntl(F_SETFL)");
  return -1;
 }

 return 0;
}

void do_read(int fd)
{
 int rc = recv(fd, buffer, sizeof(buffer), 0);
 if (rc < 0)
 {
  perror("read");
  return;
 }
 buffer[rc] = 0;
 printf("read: %s\n", buffer);
}

void do_write(int fd)
{
 static const char* greeting = "O hai!\n";
 int rc = send(fd, greeting, strlen(greeting), 0);
 if (rc < 0)
 {
  perror("write");
  return;
 }
}

void process_error(int fd)
{
 printf("fd %d error!\n", fd);
}

#define MAX_EPOLL_EVENTS 128
static struct epoll_event events[MAX_EPOLL_EVENTS];

#define BACKLOG 128

int main(int argc, char** argv)
{
 if (argc < 2)
 {
  printf("Usage: %s <port>\n", argv[0]);
  return EXIT_FAILURE;
 }

 char* p;
 int port = strtol(argv[1], &p, 10);
 if (*p)
 {
  printf("Invalid port number\n");
  return EXIT_FAILURE;
 }

 signal(SIGPIPE, SIG_IGN);

 int efd = epoll_create(MAX_EPOLL_EVENTS);

 int listenfd = socket(AF_INET, SOCK_STREAM, 0);
 if (listenfd < 0)
 {
  perror("socket");
  return EXIT_FAILURE;
 }

 setnonblocking(listenfd);

 struct sockaddr_in servaddr = {0};
 servaddr.sin_family = AF_INET;
 servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
 servaddr.sin_port = htons(port);

 if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
 {
  perror("bind");
  return EXIT_FAILURE;
 }

 if (listen(listenfd, BACKLOG) < 0)
 {
  perror("listen");
  return EXIT_FAILURE;
 }

 struct epoll_event listenev;
 listenev.events = EPOLLIN | EPOLLET;
 listenev.data.fd = listenfd;
 if (epoll_ctl(efd, EPOLL_CTL_ADD, listenfd, &listenev) < 0)
 {
  perror("epoll_ctl");
  return EXIT_FAILURE;
 }

 struct epoll_event connev;
 int events_count = 1;

 for (;;)
 {
  int nfds = epoll_wait(efd, events, MAX_EPOLL_EVENTS, -1);

  for (int i = 0; i < nfds; i++)
  {
   if (events[i].data.fd == listenfd)
   {
    int connfd = accept(listenfd, NULL, NULL);
    if (connfd < 0)
    {
     perror("accept");
     continue;
    }

    if (events_count == MAX_EPOLL_EVENTS-1)
    {
     printf("Event array is full\n");
     close(connfd);
     continue;
    }

    setnonblocking(connfd);
    connev.data.fd = connfd;
    connev.events = EPOLLIN | EPOLLOUT | EPOLLET | EPOLLRDHUP;
    if (epoll_ctl(efd, EPOLL_CTL_ADD, connfd, &connev) < 0)
    {
     perror("epoll_ctl");
     close(connfd);
     continue;
    }

    events_count++;
   }
   else
   {
    int fd = events[i].data.fd;

    if (events[i].events & EPOLLIN)
     do_read(fd);

    if (events[i].events & EPOLLOUT)
     do_write(fd);

    if (events[i].events & EPOLLRDHUP)
     process_error(fd);

    epoll_ctl(efd, EPOLL_CTL_DEL, fd, &connev);
    close(fd);
    events_count--;
   }
  }
 }

 return 0;
}
```

[3 минуты]::
### kqueue

[kqueue(2)](https://www.freebsd.org/cgi/man.cgi?query=kqueue&sektion=2)

```c
#include <sys/event.h>

struct kevent {
    uintptr_t ident;  /* identifier for this event */
    short     filter; /* filter for event */
    u_short   flags;  /* action flags for kqueue */
    u_int     fflags; /* filter flag value */
    int64_t   data;   /* filter data value */
    void*     udata;  /* opaque user data identifier */
    uint64_t  ext[4]; /* extensions */
};

int kqueue(void);
int kevent(int kq, const struct kevent* changelist, int nchanges,
    struct kevent* eventlist, int nevents,
    const struct timespec* timeout);
EV_SET(kev, ident, filter, flags, fflags, data, udata);
```

[A TCP server with kqueue](https://dev.to/frevib/a-tcp-server-with-kqueue-527)

[Scalable Event Multiplexing: epoll vs. kqueue](https://long-zhou.github.io/2012/12/21/epoll-vs-kqueue.html)

[2 минуты]::
### io_uring

![](io-uring.png)

[An Introduction to the io_uring Asynchronous I/O Framework](https://blogs.oracle.com/linux/post/an-introduction-to-the-io-uring-asynchronous-io-framework)

[Efficient IO with io_uring](https://kernel.dk/io_uring.pdf)

[io_uring: пишем убийцу Nginx](https://youtu.be/g42hzMplhkQ)

[Missing Manuals - io_uring worker pool](https://blog.cloudflare.com/missing-manuals-io_uring-worker-pool)

[2 минуты]::
### WinAPI: I/O completion ports

[I/O Completion Ports](https://docs.microsoft.com/en-us/windows/win32/fileio/i-o-completion-ports)

[The Winsock completion port model programming tutorials, info and references](https://www.winsocketdotnetworkprogramming.com/winsock2programming/winsock2advancediomethod5i.html)

[epoll и Windows IO Completion Ports: практическая разница](https://habr.com/ru/company/infopulse/blog/415403)

[4 минуты]::
### Библиотеки-обёртки

* [libevent](https://libevent.org): supports `/dev/poll`, `kqueue(2)`, POSIX `select(2)`, Windows `select()`, `poll(2)`, and `epoll(4)`. Libevent includes support for several useful protocols, including DNS, HTTP, and a minimal RPC framework.
* [libuv](https://libuv.org): primarily developed for use by Node.js, but it’s also used by Luvit, Julia, pyuv, and others.
  * Full-featured event loop backed by epoll, kqueue, IOCP
  * Asynchronous DNS resolution
  * File system events
* [libev](http://software.schmorp.de/pkg/libev.html): used in GNU Virtual Private Ethernet, rxvt-unicode, auditd, the Deliantra MORPG Server and Client, and many other programs.
  * [Redis, hiredis, libev и multithread](https://habr.com/ru/post/131916)
* [libhv](https://github.com/ithewei/libhv)
* [GNU adns](https://gnu.org/software/adns)

[5 минут]::
## Практические последствия

[The C10K problem](http://kegel.com/c10k.html)

[What is epoll & C1M problem](https://codersguild.github.io/System-Design/C1B%20Problem)

[Nginx: методы обработки соединений](https://nginx.org/ru/docs/events.html)

[Паттерн Reactor](http://design-pattern.ru/patterns/reactor.html)

![](reactor.png)

## Высокоуровневое асинхронное программирование

[9 минут]::
### Корутины

[Кооперативная многозадачность](https://ru.wikipedia.org/wiki/Кооперативная_многозадачность)

[A C++20 coroutine example](https://mariusbancila.ro/blog/2020/06/22/a-cpp20-coroutine-example):

```cpp
task<> tcp_echo_server() {
  char data[1024];
  while (true) {
    size_t n = co_await socket.async_read_some(buffer(data));
    co_await async_write(socket, buffer(data, n));
  }
}
```

[libmill](http://libmill.org): Go-style concurrency in C, [libdill](http://libdill.org): structured concurrency for C.

[libaco](https://github.com/hnes/libaco): A blazing fast and lightweight C asymmetric coroutine library.

[libtask](https://swtch.com/libtask): simple coroutine library.

[libconcurrent](https://github.com/sharow/libconcurrent): Concurrent Programming Library (Coroutine) for C11.

[libcoro](http://software.schmorp.de/pkg/libcoro.html): implements coroutines (cooperative multitasking) in a portable fashion.

[Portable Coroutine Library](http://www.xmailserver.org/libpcl.html)

<https://en.wikibooks.org/wiki/C_Programming/Coroutines>, [Coroutines in C](https://www.devever.net/~hl/coroutines), [Coroutines in one page of C](https://www.embeddedrelated.com/showarticle/455.php):

* POSIX 2001: [makecontext(3)](https://linux.die.net/man/3/makecontext), [setcontext(3)](https://linux.die.net/man/3/setcontext), [setjmp(3)](https://linux.die.net/man/3/setjmp)
* WinAPI: [Fibers](https://docs.microsoft.com/en-us/windows/win32/procthread/fibers)

[Coroutines in C](https://www.chiark.greenend.org.uk/~sgtatham/coroutines.html); [On Duff's Device and Coroutines](https://research.swtch.com/duff)

[2 минуты]::
### Futures

[Анатомия асинхронных фреймворков в C++ и других языках](https://habr.com/ru/company/yandex/blog/647853)

```cpp
#include <iostream>
#include <future>
#include <chrono>

bool is_prime(int x) {
  for (int i=2; i<x; ++i) if (x%i==0) return false;
  return true;
}

int main ()
{
  // call function asynchronously:
  std::future<bool> fut = std::async (is_prime,444444443);

  // do something while waiting for function to set future:
  std::cout << "checking, please wait";
  std::chrono::milliseconds span(100);
  while (fut.wait_for(span) == std::future_status::timeout)
    std::cout << '.' << std::flush;

  bool x = fut.get();

  std::cout << "\n444444443 " << (x?"is":"is not") << " prime.\n";

  return 0;
}
```

[Rust-style futures in C](https://axelf.nu/2020/08/24/rust-style-futures-in-c.html)

[1 минута]::
### Go channels

[Pure C implementation of Go channels](https://github.com/tylertreat/chan)

Из области курьезного: [pygolang](https://lab.nexedi.com/kirr/pygolang/blob/master/golang/libgolang.h)

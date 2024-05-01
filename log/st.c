

#define TRACE_MSG fprintf(stderr, __FUNCTION__     \
                                         "() [%s:%d] here I am\n", \
                         __FILE__, __LINE__)


#include <stdio.h>
#include <signal.h>
#include <execinfo.h>

void show_stackframe() {
  void *trace[32];
  char **messages = (char **)NULL;
  int i, trace_size = 0;

  trace_size = backtrace(trace, 32);
  messages = backtrace_symbols(trace, trace_size);
  printf("[bt] Execution path:\n");
  for (i=1; i<trace_size; ++i)
        printf("[bt] %s\n", messages[i]);
}


int func_low(int p1, int p2) {

  p1 = p1 - p2;
  show_stackframe();

  return 2*p1;
}

int func_high(int p1, int p2) {

  p1 = p1 + p2;
  show_stackframe();

  return 2*p1;
}


int test(int p1) {
  int res;

  if (p1<10)
        res = 5+func_low(p1, 2*p1);
  else
        res = 5+func_high(p1, 2*p1);
  return res;
}



int main() {

  printf("First call: %d\n\n", test(27));
  printf("Second call: %d\n", test(4));

}

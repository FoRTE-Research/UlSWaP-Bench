#ifndef PICOLIBC_FLOAT_PRINTF_SCANF
#define printf_float(_num) _num
#endif  // PICOLIBC_FLOAT_PRINTF_SCANF

#ifdef BARE_METAL

//#include <stdio.h>
int benchmark();

#define RUNS 1

int main(void)
{
  int run;

  // Run baseline
  //puts("Baseline\n\r");
  //benchmark();
  for(run = 0; run < RUNS; ++run)
  {
    //puts("Trial\n\r");
    //printf("Run %d\n\r", run+1);
    benchmark();
  }

  return 0;
}

#define printf(...)
#define fprintf(...)
#define fflush(...)
#define main(...) benchmark(__VA_ARGS__)

#endif

#ifdef BARE_METAL

//#include <stdio.h>
int benchmark();

#define RUNS 0x0FFFFFFF

extern void initLED();
extern void LED();

int main(void)
{
  int run = 0;
  volatile int delay = 0;

  // Run baseline
  //puts("Baseline\n\r");
  benchmark();
  while(1);
  //for(run = 0; run < RUNS; ++run)
  //{
    //puts("Trial\n\r");
    //printf("Run %d\n\r", run+1);
    //LED(run + 1);
    //benchmark();
    //LED(run);
    //for (delay = 0; delay < 0x340850; ++delay);
  //}
  //LED(0);

  return 0;
}

#define printf(...)
#define fprintf(...)
#define fflush(...)
#define main(...) benchmark(__VA_ARGS__)

#endif

/* timestamp example */
#include <stdio.h>      /* printf */
#include <time.h>       /* time_t, time (for timestamp in second) */
#include <sys/time.h>   /* gettimeofday, timeval (for timestamp in microsecond) */
 
int time (void)
{
  /* Example of timestamp in microsecond. */
  struct timeval timer_usec; 
  long long int timestamp_usec; /* timestamp in microsecond */
  if (!gettimeofday(&timer_usec, NULL)) {
    timestamp_usec = ((long long int) timer_usec.tv_sec) * 1000000ll + 
                        (long long int) timer_usec.tv_usec;
  }
  else {
    timestamp_usec = -1;
  }
  printf("%lld microseconds since epoch\n", timestamp_usec);

  return 0;
}
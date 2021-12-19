#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char const *argv[])
{   
    cpu_set_t set;
    CPU_ZERO(&set);        // clear cpu mask
    CPU_SET(0, &set);      // set cpu 0
    sched_setaffinity(0, sizeof(cpu_set_t), &set);  // 0 is the calling process

    int NUMPAGES;
    scanf("%d", &NUMPAGES);
    size_t PAGESIZE = (size_t) sysconf(_SC_PAGESIZE);
    int *a = (int*) malloc(PAGESIZE*NUMPAGES);
    int jump = PAGESIZE / sizeof(int);

    // cache pages in tlb
    for (int i = 0; i < NUMPAGES * jump; i += jump)
        a[i] = 0;

    // measure size of tlb
    struct timespec requestStart, requestEnd;
    clock_gettime(CLOCK_REALTIME, &requestStart);
    for (int i = 0; i < NUMPAGES * jump; i += jump)
        a[i] += 1;
    clock_gettime(CLOCK_REALTIME, &requestEnd);

    double total_time = ( requestEnd.tv_sec - requestStart.tv_sec )
      + ( requestEnd.tv_nsec - requestStart.tv_nsec )
      / 1E9;

    double total = total_time/(NUMPAGES*jump);

    printf("%f", total);
    return 0;
}
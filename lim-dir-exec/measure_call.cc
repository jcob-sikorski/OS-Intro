#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    struct timeval stop, start;
    gettimeofday(&start, NULL);
    for (int i = 0; i < 10000; i++) {
        int *byte = (int*) malloc(100);
        free(byte);
    }
    gettimeofday(&stop, NULL);
    printf("took %f us\n", double (stop.tv_usec - start.tv_usec)/10000);
    return 0;
}

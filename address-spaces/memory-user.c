#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    unsigned long long int size;
    unsigned long long int mb = 1024*1024;
    unsigned long long int arr_size;
    char* arr;
    time_t start, diff;

    printf("size (MB): ");
    scanf("%llu", &size);
    printf("time (s): ");
    scanf("%llu", &diff);

    arr_size = size * mb;
    arr = (char*) malloc(arr_size);

    start = time(NULL);

    while (time(NULL) - start < diff) {
        int i = 0;
        for (unsigned long long int i; i < arr_size; i++) {
            continue;
        }
    }
    free(arr);
    return 0;
}

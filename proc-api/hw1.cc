#include<unistd.h>
#include<stdio.h>

int main(int argc, char const *argv[])
{
    int x  = 100;
    fork();
    fprintf(stderr, "%d", x);
    return 0;
}

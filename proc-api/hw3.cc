#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    int rc = fork();

    if (rc == 0) {
        printf("hello\n");
    }
    else if (rc > 0) {
        wait(&rc);
        printf("goodbye\n");
    }

    return 0;
}
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    int rc = fork();

    if (rc == 0) {
        close(STDOUT_FILENO);
        printf("doing check...\n");
    }
    return 0;
}

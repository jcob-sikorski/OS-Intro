#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    int rc = fork();
    
    if (rc == 0) {
        // printf("%d child\n", wait(&rc));
        printf("child\n");
    }
    else if (rc > 0) {
        printf("%d parent\n", wait(&rc)); 
        // printf("parent\n");
    }
    return 0;
}

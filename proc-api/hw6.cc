#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    int pid = fork();
    int status;
    
    if (pid == 0) {
        printf("%d child\n", waitpid(pid, &status, WEXITED)); 
        // printf("child\n");
    }
    else if (pid > 0) {
        // printf("%d parent\n", waitpid(pid, &status, WEXITED)); 
        printf("parent\n");
    }
    return 0;
}

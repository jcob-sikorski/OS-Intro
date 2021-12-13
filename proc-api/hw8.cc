#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char const *argv[])
{
    int fd1[2];
    int fd2[2];

    if (pipe(fd1)==-1) {
        fprintf(stderr, "Pipe Failed" );
        exit(EXIT_FAILURE);
    }
    if (pipe(fd2)==-1) {
        fprintf(stderr, "Pipe Failed" );
        exit(EXIT_FAILURE);
    }

    pid_t rc1 = fork();
    pid_t rc2 = fork();

    if (rc1 == 0) {
        char *input_str = "Redirected input from child";

        write(fd1[1], input_str, strlen(input_str)+1);
        close(fd1[1]);

        wait(&rc2);
        close(fd2[1]);
    }
    else if (rc2 == 0) {
        int status;
        waitpid(rc1, &status, WEXITED);

        char *input_str = (char*) malloc(100);

        read(fd1[0], input_str, 100); // 29 really
        close(fd1[0]);

        write(fd2[1], input_str, strlen(input_str)+1);
        close(fd2[1]);
    }
    else if (rc1 > 0 || rc2 > 0) {
        int status;
        waitpid(rc2, &status, WEXITED);

        char *input_str = (char*) malloc(100);

        read(fd2[0], input_str, 100); // 29 really
        close(fd2[0]);

        fprintf(stderr, input_str);        
    }


    return 0;
}

#include<fcntl.h>
#include<unistd.h>
#include<string.h>

int main(int argc, char const *argv[])
{
    int fd = open("/Users/jakubsiekiera/Dev/OS-Intro/proc-api/hw1", O_WRONLY);
    fork();
    write(fd, "I am buffer.", strlen("I am buffer."));
    return 0;
}

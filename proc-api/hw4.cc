#include <unistd.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    int rc = fork();
    execl("/bin/ls", ".");
    return 0;
}

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char** argv)
{
    int number = 777;
    unsigned long long int i;
    int fd = open("big_file", O_WRONLY | O_TRUNC);
    for (i = 0; i < 10000000; i++)
        write(fd, &number, sizeof(int));
    close(fd);

    return 0;
}
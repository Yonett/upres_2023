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
    int result = 0;
    int fd = open("big_file", O_RDONLY);
    while (read(fd, &result, sizeof(int)) == sizeof(int))
    {
        ;
    }
    close(fd);
    return 0;
}
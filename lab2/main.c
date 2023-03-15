#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>

int main(int argc, char** argv)
{
    if (argc != 3)
    {
      fprintf(stderr, "Error! Wrong number of arguments (expected 2, given %d).\n", --argc);
      exit(EXIT_FAILURE);
    }

    pid_t pidNM = fork();
 
    if (pidNM == -1)
        fprintf(stderr, "Main process unable to fork\n");
    else if (pidNM > 0)
    {
        int status;
        wait(&status);
        int fd, nFactorial, nmFactorial;

        fd = open("data", O_RDONLY);
        if (read(fd, &nFactorial, sizeof(int)) != sizeof(int) || read(fd, &nmFactorial, sizeof(int)) != sizeof(int))
            {
                close(fd);
                exit(EXIT_FAILURE);
            }
        close(fd);

        printf("Number of permutations without repetitions: %d\n", (nFactorial / nmFactorial));
    }
    else
    {
        pid_t pidN = fork();
        if (pidN == -1)
            fprintf(stderr, "Process NM unable to fork\n");
        else if (pidN > 0)
        {
            int status;
            wait(&status);
            if (execl("factorial.o", argv[1], argv[2], NULL) == -1)
            {
                fprintf(stderr, "Process NM unable to exec facrotial.o\n");
                exit(EXIT_FAILURE);
            }
            exit(EXIT_SUCCESS);
        }
        else
            if (execl("factorial.o", argv[1], NULL) == -1)
            {
                fprintf(stderr, "Process N unable to exec facrotial.o\n");
                exit(EXIT_FAILURE);
            }
            exit(EXIT_SUCCESS);
    }
    return 0;
}
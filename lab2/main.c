#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>

int main(int argc, char** argv)
{
    if (argc != 3)
    {
      fprintf(stderr, "Error! Wrong number of arguments (expected 2, given %d).\n", --argc);
      exit(1);
    }

    pid_t pid1 = fork();
 
    if (pid1 == -1)
        fprintf(stderr, "Unable to fork\n");
    else if (pid1 > 0)
    {
        int status;
        waitpid(pid1, &status, 0);
        FILE *file;
        int factorial1, factorial2;
        file = fopen("data", "r");
        fscanf(file, "%d\n%d", &factorial1, &factorial2);
        fclose(file);
        printf("Number of permutations without repetitions: %d\n", (factorial1 / factorial2));
    }
    else
    {
        pid_t pid2 = fork();
        if (pid2 == -1)
            fprintf(stderr, "Unable to fork\n");
        else if (pid2 > 0)
        {
            int status;
            waitpid(pid2, &status, 0);
            if (execl("factorial.o", argv[1], argv[2], NULL) == -1)
                fprintf(stderr, "Unable to exec\n");
        }
        else
            if (execl("factorial.o", argv[1], NULL) == -1)
                fprintf(stderr, "Unable to exec\n");
    }
    return 0;
}
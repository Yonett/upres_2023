#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>

/**
 * Функция считает факториал целого числа.
 * @param n Число, факториал которого нужно рассчитать.
 * @return Значение факториала числа.
 **/
int factorial (int n)
{
    int i, result = 1;
    for (i = 2; i <= n; ++i)
        result *= i;
    return result;
}

int main(int argc, char** argv)
{
    // if (argc != 3)
    // {
    //   printf("Error! Wrong number of arguments (expected 2, given %d).\n", --argc);
    //   exit(0);
    // }

    // long n, m;
    // char *endptr;

    // n = strtol(argv[1], &endptr, 10);
    // if (*endptr != '\0')
    // {
    //     printf("Error! Cannot convert fisrt argument (%s) to integer.\n", argv[1]);
    //     exit(0);
    // }

    // m = strtol(argv[2], &endptr, 10);
    // if (*endptr != '\0')
    // {
    //     printf("Error! Cannot convert second argument (%s) to integer.\n", argv[2]);
    //     exit(0);
    // }
    
    // printf("n: %d\nm: %d\n", n, m);
    // printf("%d\n", (factorial(n) / factorial(n-m)));
    // execl("factorial.o", NULL);

    // FILE *file;

    // int num;

    // file = fopen("data", "r");

    // fscanf(file, "%d", &num);
    // printf("%d\n", num);

    // fscanf(file, "%d", &num);
    // printf("%d\n", num);
    
    // fclose(file);

    pid_t pid1 = fork();
    pid_t pid2 = fork();
 
    if (pid1 == -1 || pid2 == -1)
    {
        fprintf(stderr, "Unable to fork\n");
    }
    else if (pid1 > 0)
    {
        printf("I am parent %d\n", getpid());
        printf("Child is %d\n", pid1);
        int status;
        // wait(&status);
        waitpid(pid1, &status, 0);
    }
    else if (pid1 == 0)
    {
        // we are the child
        printf("I am child %d of %d\n", getpid(), getppid());
        if (execl("factorial.o", NULL) == -1)
        {
            fprintf(stderr, "Unable to exec\n");
        }
    }

    return 0;
}
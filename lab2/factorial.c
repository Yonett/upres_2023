#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
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
    if (argc == 1)
    {
        int n, fd, result;
        char *endptr;

        n = strtol(argv[0], &endptr, 10);
        if (*endptr != '\0')
        {
            fprintf(stderr, "Error! ProcessN cannot convert fisrt argument (%s) to integer.\n", argv[0]);
            truncate("data", 0);
            return(EXIT_FAILURE);
        }

        result = factorial(n);
        fd = open("data", O_TRUNC | O_WRONLY);
        write(fd, &result, sizeof(int));
        close(fd);
    }
    else if (argc == 2)
    {
        int n, m, fd, result;
        char *endptr;

        n = strtol(argv[0], &endptr, 10);
        if (*endptr != '\0')
        {
            fprintf(stderr, "Error! ProcessNM cannot convert fisrt argument (%s) to integer.\n", argv[0]);
            exit(EXIT_FAILURE);
        }

        m = strtol(argv[1], &endptr, 10);
        if (*endptr != '\0')
        {
            fprintf(stderr, "Error! ProcessNM cannot convert second argument (%s) to integer.\n", argv[1]);
            truncate("data", 0);
            exit(EXIT_FAILURE);
        }

        result = factorial(n - m);
        fd = open("data", O_WRONLY | O_APPEND);
        write(fd, &result, sizeof(int));
        close(fd);
    }
    return 0;
}
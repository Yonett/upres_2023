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
    if (argc == 1)
    {
        int n;
        char *endptr;

        n = strtol(argv[0], &endptr, 10);
        if (*endptr != '\0')
        {
            printf("Error! Cannot convert fisrt argument (%s) to integer.\n", argv[1]);
            exit(0);
        }

        FILE *file;
        file = fopen("data", "w");
        fprintf(file, "%d\n", factorial(n));
        fclose(file);
    }
    else if (argc == 2)
    {
        int n, m;
        char *endptr;

        n = strtol(argv[0], &endptr, 10);
        if (*endptr != '\0')
        {
            printf("Error! Cannot convert fisrt argument (%s) to integer.\n", argv[1]);
            exit(0);
        }

        m = strtol(argv[1], &endptr, 10);
        if (*endptr != '\0')
        {
            printf("Error! Cannot convert second argument (%s) to integer.\n", argv[2]);
            exit(0);
        }

        FILE *file;
        file = fopen("data", "a");
        fprintf(file, "%d\n", factorial(n-m));
        fclose(file);
    }
    else
    {
        printf("Error! Wrong number of arguments (expected 2 or 1, given %d).", argc);
        exit(0);
    }
    return 0;
}
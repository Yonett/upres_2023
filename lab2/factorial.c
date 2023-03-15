#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

/**
 * Функция считает факториал целого числа.
 * @param n Число, факториал которого нужно рассчитать.
 * @return Значение факториала числа.
 **/
int factorial(int n)
{
    int i, result = 1;
    for (i = 2; i <= n; ++i)
        result *= i;
    return result;
}

int main(int argc, char** argv)
{
    if (argc == 1) //Для расчёта n!
    {
        int n, fd, result;
        char *endptr;

        n = strtol(argv[0], &endptr, 10); //Конвертирование строки с аргументом к типу integer.
        if (*endptr != '\0')
        {
            fprintf(stderr, "Error! ProcessN cannot convert fisrt argument (%s) to integer.\n", argv[0]);
            return(EXIT_FAILURE);
        }

        result = factorial(n); //Расчёт n!
        fd = open("data", O_TRUNC | O_WRONLY); //Открываем файл для записи с изменением длины файла до 0
        write(fd, &result, sizeof(int)); //Записываем в файл значение n!
        close(fd); //Закрываем файл.
    }
    else if (argc == 2) //Для расчёта (n - m)!
    {
        int n, m, fd, result;
        char *endptr;

        n = strtol(argv[0], &endptr, 10); //Конвертирование строки с первым аргументом к типу integer.
        if (*endptr != '\0')
        {
            fprintf(stderr, "Error! ProcessNM cannot convert fisrt argument (%s) to integer.\n", argv[0]);
            exit(EXIT_FAILURE);
        }

        m = strtol(argv[1], &endptr, 10); //Конвертирование строки со вторым аргументом к типу integer.
        if (*endptr != '\0')
        {
            fprintf(stderr, "Error! ProcessNM cannot convert second argument (%s) to integer.\n", argv[1]);
            exit(EXIT_FAILURE);
        }

        result = factorial(n - m); //Расчёт (n - m)!
        fd = open("data", O_WRONLY | O_APPEND); //Открываем файл для записи в режиме добавления.
        write(fd, &result, sizeof(int)); //Записываем в файл значение (n - m)!
        close(fd); //Закрываем файл.
    }
    return 0;
}
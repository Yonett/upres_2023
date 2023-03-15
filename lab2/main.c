#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main(int argc, char** argv)
{
    if (argc != 3)
    {
      fprintf(stderr, "Error! Wrong number of arguments (expected 2, given %d).\n", --argc);
      exit(EXIT_FAILURE);
    }

    pid_t pidNM = fork(); //Процесс, который будет считать (n - m)!
 
    if (pidNM == -1) //Если не удалось создать процесс.
        fprintf(stderr, "Main process unable to fork.\n");
    else if (pidNM > 0) //Родительский процесс. 
    {
        int status;
        wait(&status); //Ожидает завершения процесса NM.
        if (status != EXIT_SUCCESS) //Если процесс NM завершился с ошибкой.
            exit(EXIT_FAILURE);

        int fd, nFactorial, nmFactorial;

        fd = open("data", O_RDONLY); //Открываем файл с данными для чтения.
        if (read(fd, &nFactorial, sizeof(int)) != sizeof(int) ||
            read(fd, &nmFactorial, sizeof(int)) != sizeof(int)) //Проверяем, чтобы считалось нужное количество байт (два раза по int).
        {
            close(fd); //Закрываем файл.
            exit(EXIT_FAILURE); 
        }
        close(fd); //Закрываем файл.

        printf("Number of permutations without repetitions: %d.\n", (nFactorial / nmFactorial)); //Вывод результата
    }
    else //Процесс NM
    {
        pid_t pidN = fork(); //Процесс, который будет считать n!
        if (pidN == -1) //Если не удалось создать процесс.
            fprintf(stderr, "Process NM unable to fork.\n");
        else if (pidN > 0) //Родительский процесс.
        {
            int status;
            wait(&status); //Ожидает завершения процесса N.
            if (status != EXIT_SUCCESS) //Если процесс N завершился с ошибкой.
                exit(EXIT_FAILURE);
            if (execl("factorial.o", argv[1], argv[2], NULL) == -1) //Запуск программы для расчёта (n - m)!
            {
                fprintf(stderr, "Process NM unable to exec factorial.o (%s).\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
            exit(EXIT_SUCCESS);
        }
        else //Процесс N.
        {
            if (execl("factorial.o", argv[1], NULL) == -1) //Запуск программы для расчёта n!
            {
                fprintf(stderr, "Process N unable to exec factorial.o (%s).\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
            exit(EXIT_SUCCESS);
        }
    }
    return 0;
}
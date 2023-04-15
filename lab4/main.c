#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>

/* cat a.txt b.txt c.txt | tr -d [0-9] | wc -w */

int main(int argc, char** argv)
{
    // P0 выполняет "wc -w"

    int fd1[2], status;
    pipe(fd1); //Канал для общения между P0 и P1

    if (fork() == 0)
    {
        // P1 выполняет "tr -d [0-9]"

        int fd2[2];
        pipe(fd2); // Канал для общения между P1 и P2

        if (fork() == 0)
        {
            // P2 выполняет "cat a.txt b.txt c.txt"

            close(fd1[0]); //
            close(fd1[1]); // Закрытие не используемых декрипторов
            close(fd2[0]); //

            close(1); // Закрытие стандартного вывода
            dup(fd2[1]); // Переназначение стандартного вывода дескриптором записи канала fd2
            close(fd2[1]); // Закрытие старого дескриптора записи канала fd2

            if (execl("/bin/cat", "cat", "a.txt", "b.txt", "c.txt", NULL) == -1)
                exit(EXIT_FAILURE);
            exit(EXIT_SUCCESS);
        }

        wait(&status);

        if (status != EXIT_SUCCESS)
        {
            fprintf(stderr, "Error! Cannot execute cat\n");
            exit(EXIT_FAILURE);
        }

        close(fd1[0]); //
        close(fd2[1]); // Закрытие не используемых декрипторов

        close(1); // Закрытие стандартного вывода
        dup(fd1[1]); // Переназначение стандартного вывода дескриптором записи канала fd1
        close(fd1[1]); // Закрытие старого дескриптора записи канала fd1

        close(0); // Закрытие стандартного ввода
        dup(fd2[0]); // Переназначение стандартного вывода дескриптором чтения канала fd2
        close(fd2[0]); // Закрытие старого дескриптора чтения канала fd2

        if (execl("/bin/tr", "tr", "-d", "[0-9]", NULL) == -1)
            exit(EXIT_FAILURE);
        exit(EXIT_SUCCESS);
    }

    wait(&status);

    if (status != EXIT_SUCCESS)
    {
        fprintf(stderr, "Error! Cannot execute tr\n");
        exit(EXIT_FAILURE);
    }

    close(fd1[1]); // Закрытие неиспользуемого дескриптора

    close(0); // Закрытие стандартного ввода
    dup(fd1[0]); // Переназначение стандартного вывода дескриптором чтения канала fd1
    close(fd1[0]); // Закрытие старого дескриптора чтения канала fd1

    if (execl("/bin/wc", "wc", "-w", NULL) == -1)
    {
        fprintf(stderr, "Error! Cannot execute wc\n");
        exit(EXIT_FAILURE);
    }
    return 0;
}
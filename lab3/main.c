#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>

/**
 * Функция-обработчик сигнала SIGLARM
**/
void wakeup()
{
    signal(SIGALRM, wakeup);
}

int main(int argc, char** argv)
{
    int k1[2];
    pipe(k1); //Канал для передачи сообщений между процессами

    signal(SIGALRM, wakeup); //Назначение обработчика сигнала SIGALRM

    pid_t p1 = fork();

    if (p1 == -1)
    {
        fprintf(stderr, "P0: Unable to fork P1.\n");
        exit(EXIT_FAILURE);
    }
    else if (p1 > 0)    //Процесс P0
    {
        char recievedMsg[40]; //Полученное сообщение
        int i, status, spid; //spid - sender process id

        for (i = 0; i < 3; i++) //Планируется получить 3 сообщения
        {
            read(k1[0], &spid, sizeof(int));
            read(k1[0], &recievedMsg, 40);
            printf("%d: %s\n", spid, recievedMsg);
        }

        wait(&status);
        exit(EXIT_SUCCESS);
    }
    else    //Процесс P1
    {
        pid_t p2 = fork();
        if (p2 == -1)
        {
            fprintf(stderr, "P1: Unable to fork P2.\b");
            exit(EXIT_FAILURE);
        }
        else if (p2 > 0) // Процесс P1
        {
            char msg[40];
            pid_t pid = getpid();
            int status;

            sprintf(msg, "Process is created\0");

            write(k1[1], &pid, sizeof(int)); //Отправка сообщения через канал
            write(k1[1], &msg, 40);

            alarm(3); //Будильник на 3 секунды
            pause();

            if (kill(p2, SIGINT) == -1) //Отправка SIGINT дочернему процессу
                fprintf(stderr, "%s\n", strerror(errno));

            wait(&status); //Ожидание завершения дочернего процесса

            sprintf(msg, "Process %d ended with status %d\0", p2, status);
            write(k1[1], &pid, sizeof(int)); //Отправка сообщения через канал
            write(k1[1], &msg, 40);

            exit(EXIT_SUCCESS);
        }
        else    //  Процесс P2
        {
            int bf = open("big_file", O_RDONLY); //Открытие файла большого размера
            int number;
            char msg[40];
            pid_t pid = getpid();

            sprintf(msg, "Process start reading\0");

            write(k1[1], &pid, sizeof(int)); //Отправка сообщения через канал
            write(k1[1], &msg, 40);
            while(true)
                read(bf, &number, sizeof(int)); //Чтение из файла, пока процесс не получит сигнал
        }
    }
    return 0;
}
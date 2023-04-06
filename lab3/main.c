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

bool doReading = true; //Переменная-флаг для чтения файла

/**
 * Функция-обработчик сигнала SIGLARM
**/
void sigALRMHandler()
{
    signal(SIGALRM, sigALRMHandler);
}

/**
 * Функция-обработчик сигнала SIGINT
**/
void sigINTHandler()
{
    doReading = false; //Останавливаем чтение файла
    signal(SIGINT, sigINTHandler);
}

int main(int argc, char** argv)
{
    int k1[2];
    pipe(k1); //Канал для передачи сообщений между процессами

    signal(SIGALRM, sigALRMHandler); //Назначение обработчика сигнала SIGALRM
    signal(SIGINT, sigINTHandler); //Назначение обработчика сигнала SIGINT

    pid_t p1 = fork();

    if (-1 == p1)
    {
        fprintf(stderr, "P0: Unable to fork P1.\n");
        exit(EXIT_FAILURE);
    }
    else if (p1 > 0)    //Процесс P0
    {
        printf("P1 sucessfully created\n");

        char recievedMsg[40]; //Буфер для полученного сообщение
        int i, status, spid; //spid - sender process id

        for (i = 0; i < 3; i++) //Планируется получить 3 сообщения
        {
            if (read(k1[0], &spid, sizeof(int)) == sizeof(int))
                printf("P0 succesfully read a pid from pipe\n");
            else
                fprintf(stderr, "P0 cannot read a pid from pipe\n");

            if (read(k1[0], &recievedMsg, 40) == 40)
                printf("P0 succesfully read a message from pipe\n");
            else
                fprintf(stderr, "P0 cannot read a message from pipe\n");

            printf("%d: %s\n", spid, recievedMsg);
        }

        wait(&status); //Ожидание завершения процесса P1

        if (status == EXIT_FAILURE)
            exit(EXIT_FAILURE);

        exit(EXIT_SUCCESS);
    }
    else    //Процесс P1
    {
        pid_t p2 = fork();
        if (-1 == p2)
        {
            fprintf(stderr, "P1: Unable to fork P2.\b");
            exit(EXIT_FAILURE);
        }
        else if (p2 > 0) // Процесс P1
        {
            printf("P2 sucessfully created\n");
            char msg[40];
            pid_t pid = getpid();
            int status;

            sprintf(msg, "(P1) Process is created\0");

            if (write(k1[1], &pid, sizeof(int)) == sizeof(int)) //Отправка сообщения через канал
                printf("P1 succesfully wrote own pid to pipe\n");
            else
                fprintf(stderr, "P1 cannot write own pid to pipe\n");

            if (write(k1[1], &msg, 40) == 40)
                printf("P1 succesfully wrote own message to pipe\n");
            else
                fprintf(stderr, "P1 cannot write own message to pipe\n");

            alarm(3); //Будильник на 3 секунды
            printf("P1 turn on an alarm for 3 seconds\n");
            pause();
            printf("P1 is awake\n");

            if (-1 == kill(p2, SIGINT)) //Отправка SIGINT дочернему процессу
                fprintf(stderr, "%s\n", strerror(errno));
            else
                printf("P1 succesfully sent SIGINT to P2\n");

            wait(&status); //Ожидание завершения дочернего процесса

            sprintf(msg, "(P1) Process %d ended with status %d\0", p2, status);

            if (write(k1[1], &pid, sizeof(int)) == sizeof(int)) //Отправка сообщения через канал
                printf("P1 succesfully wrote own pid to pipe\n");
            else
                fprintf(stderr, "P1 cannot write own pid to pipe\n");

            if (write(k1[1], &msg, 40) == 40)
                printf("P1 succesfully wrote own message to pipe\n");
            else
                fprintf(stderr, "P1 cannot write own message to pipe\n");

            exit(EXIT_SUCCESS);
        }
        else    //  Процесс P2
        {
            int bf = open("big_file", O_RDONLY); //Открытие файла большого размера

            if (-1 == bf)
            {
                fprintf(stderr, "P2 cannot open big file\n");
                exit(EXIT_FAILURE);
            }
            else
                printf("P2 succesfully opened big file\n");

            int number, i = 0;
            char msg[40];
            pid_t pid = getpid();

            printf("P2 started reading file\n");

            while(doReading)
                if (read(bf, &number, sizeof(int)) != sizeof(int)) //Чтение из файла, пока процесс не получит сигнал
                    doReading = false;
                else
                    i++;

            printf("P2 finished reading file\n");

            sprintf(msg, "(P2) Process read %d times\0", i);

            if (write(k1[1], &pid, sizeof(int)) == sizeof(int)) //Отправка сообщения через канал
                printf("P2 succesfully wrote own pid to pipe\n");
            else
                fprintf(stderr, "P2 cannot write own pid to pipe\n");

            if (write(k1[1], &msg, 40) == 40)
                printf("P2 succesfully wrote own message to pipe\n");
            else
                fprintf(stderr, "P2 cannot write own message to pipe\n");

            exit(EXIT_SUCCESS);
        }
    }
    return 0;
}
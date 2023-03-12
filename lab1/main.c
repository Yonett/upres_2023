#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

/**
 * Структура, которая содержит нужную по заданию
 * информацию о файле
**/
struct file_info
{
   char* name; //Имя файла.
   off_t st_size; // Размер в байтах.
   time_t time; // Время последней модификации.
   nlink_t st_nlink; // Количество жёстких ссылок.
};

/**
 * Функция сортирует массив строк в алфавитном порядке
 * при помощи сортировки пузырьком.
 * @param array Массив, который нужно отсортировать.
 * @param size Размер массива.
 **/
void sortInfos(struct file_info array[], int size)
{
   struct file_info tmp;
   int i, j;

   for (i = 1; i < size; i++)
      for (j = 0; j < size - i; j++)
         if (strcmp(array[j].name, array[j + 1].name) > 0)
         {
            tmp = array[j];
            array[j] = array[j + 1];
            array[j + 1] = tmp;
         }
}

int main(int argc, char** argv)
{
   DIR* d;

   struct dirent* dirElement;
   struct stat buff;

   char path[50];
   struct file_info regularFiles[50];

   int filesCount = 0;
   int i;

   if (argc != 2)
   {
      fprintf(stderr, "Error! Wrong number of arguments (expected 1, given %d).\n", --argc);
      exit(1);
   }

   d = opendir(argv[1]);
   if (!d)
   {
      fprintf(stderr, "Error! %s cannot be opened.\n", argv[1]);
      closedir(d);
      exit(1);
   }

   while ((dirElement = readdir(d)) != NULL) // Чтение файлов из заданной директории.
      if (dirElement->d_name[0] != '.') //Пропуск скрытых файлов.
      {
         sprintf(path, "%s/%s", argv[1], dirElement->d_name); //Формируем путь к файлу, для функции stat().
         stat(path, &buff); //Получаем информацию о файле.
         if (S_ISDIR(buff.st_mode)) //Проверка является ли файл директорией.
            printf("Directory - %s\n", dirElement->d_name); //Печать имени файла.
         else if (S_ISREG(buff.st_mode)) //Проверка является ли файл директорией.
         {
            regularFiles[filesCount].name = dirElement->d_name;
            regularFiles[filesCount].st_size = buff.st_size;
            regularFiles[filesCount].time = buff.st_mtime;
            regularFiles[filesCount].st_nlink = buff.st_nlink;

            filesCount++;
         }
      }

   sortInfos(regularFiles, filesCount); //Сортируем массив структур информациии о файле по алфавиту.
   printf("\n");

   for (i = 0; i < filesCount; i++)
   {
      printf("%s\n", regularFiles[i].name);
      printf("Size - %ld bytes\nTime -  %sLinks - %ju\n\n",
            regularFiles[i].st_size,
            ctime(&regularFiles[i].time),
            regularFiles[i].st_nlink); //Печатаем нужную информацию о файле.
   }

   closedir(d);
   return 0;
}

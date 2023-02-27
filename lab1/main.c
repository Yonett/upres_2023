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
 * Функция сортирует массив строк в алфавитном порядке
 * при помощи сортировки пузырьком.
 * @param array Массив, который нужно отсортировать.
 * @param size Размер массива.
 **/
void sortNames(char* array[], int size)
{
   char *str;
   int i, j;

   for (i = 1; i < size; i++)
      for (j = 0; j < size - i; j++)
         if (strcmp(array[j], array[j + 1]) > 0)
         {
            str = array[j];
            array[j] = array[j + 1];
            array[j + 1] = str;
         }
}

int main(int argc, char** argv)
{
   DIR* d;

   struct dirent* dirElement;
   struct stat buff;

   char path[50];
   char* files[50];

   int filesCount = 0;
   int i;

   if (argc != 2)
   {
      printf("Error! Wrong number of arguments (expected 1, given %d).\n", --argc);
      exit(0);
   }

   d = opendir(argv[1]);
   if (!d)
   {
      printf("Error! %s cannot be opened.\n", argv[1]);
      closedir(d);
      exit(0);
   }

   while ((dirElement = readdir(d)) != NULL) // Чтение файлов из заданной директории.
      if (dirElement->d_name[0] != '.') //Пропуск скрытых файлов.
         if (dirElement->d_type == DT_DIR) //Если очередной файл - директория,
            printf("Directory - %s\n", dirElement->d_name); //то просто печатаем её имя.
         else //Если нет,
         {
            files[filesCount] = dirElement->d_name; // то складываем имена файлов в массив для сортировки.
            filesCount++;
         }

   sortNames(files, filesCount); //Сортируем массив имён файлов по алфавиту.

   printf("\n");

   for (i = 0; i < filesCount; i++)
   {
      sprintf(path, "%s/%s", argv[1], files[i]); //Формируем путь к файлу, для функции stat().
      printf("File - %s\n", files[i]); //Печатаем имя файла.
      stat(path, &buff); //Получаем информацию о файле.
      printf("Size - %ld bytes\nTime -  %sLinks - %ju\n\n", buff.st_size, ctime(&buff.st_mtime), buff.st_nlink); //Печатаем нужную информацию о файле.
   }

   closedir(d);
   return 0;
}

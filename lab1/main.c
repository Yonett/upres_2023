#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

int main(int argc, char **argv)
{
   DIR *d; //
   char cwd[512];
   char path[50] = ""; // Полный путь файла
   struct dirent *filedir; // Файл
   struct dirent *filenodirs[50]; // Массив файлов
   struct dirent *str; // Для алфавитной сортировки
   int a = 0;
   // struct List *filenodirs;
   if (argv[1] == NULL) // Задана ли директория
   {
      printf("Error! Directory no specified!\n");
      exit(0);
   }
   d = opendir(argv[1]);

   if (!d) // Существует ли директория
   {
      printf("Error! No such directory!\n");
      exit(0);
   }

   while ((filedir = readdir(d)) != NULL) // Чтение файлов из заданной директории
   {
      if (filedir->d_type == DT_DIR && filedir->d_name[0] != '.') // Если директория
      {
         printf("%s\n", filedir->d_name);
      }
      if (filedir->d_type != DT_DIR) // Если не директория
      {
         filenodirs[a] = filedir; // Складываем все файлы в массив лля сортировки
         a++;
      }
   }
   int i, j;
   for (i = 1; i < a; i++) // Сортируем файлы в алфавитном порядке
      for (j = 0; j < a - i; j++)
         if (strcmp(filenodirs[j]->d_name, filenodirs[j + 1]->d_name) > 0)
         {
            str = filenodirs[j];
            filenodirs[j] = filenodirs[j + 1];
            filenodirs[j + 1] = str;
         }
   for (i = 0; i < a; i++) // Вывод файлов
   {
      strcpy(path, "");
      strcat(path, "./");
      strcat(path, argv[1]);
      strcat(path, "/");
      printf("%s ", filenodirs[i]->d_name);
      strcat(path, filenodirs[i]->d_name); // path - полный путь к файлу
      struct stat buff;
      stat(path, &buff); // Для получения размера файла, даты изменения и кол-ва внутренних ссылок
      printf("Размер - %ld bytes, Время -  %s, Кол-во ссылок - %ju\n", buff.st_size, ctime(&buff.st_mtime), buff.st_nlink);
   }
   closedir(d);
   return 0;
}

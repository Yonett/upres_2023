#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

void sortNames(char *array[], int size)
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

int main(int argc, char **argv)
{
   struct dirent *dirElement;

   DIR *d;

   char path[50];
   char *files[50];
   char *str;

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
      printf("Error! %s is not a directory!\n", argv[1]);
      closedir(d);
      exit(0);
   }

   while ((dirElement = readdir(d)) != NULL) // Чтение файлов из заданной директории
      if (dirElement->d_name[0] != '.')
         if (dirElement->d_type == DT_DIR)
            printf("Directory - %s\n", dirElement->d_name);
         else
         {
            files[filesCount] = dirElement->d_name; // Складываем все файлы в массив лля сортировки
            filesCount++;
         }

   sortNames(files, filesCount);

   printf("\n");

   for (i = 0; i < filesCount; i++)
   {
      sprintf(path, "%s/%s", argv[1], files[i]);
      printf("File - %s\n", files[i]);
      struct stat buff;
      stat(path, &buff);
      printf("Size - %ld bytes\nTime -  %sLinks - %ju\n\n", buff.st_size, ctime(&buff.st_mtime), buff.st_nlink);
   }

   closedir(d);
   return 0;
}

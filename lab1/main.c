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
   struct dirent *filedir;

   DIR *d;

   char path[50];
   char *filenodirs[50];
   char *str;

   int files_count = 0;

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

   while ((filedir = readdir(d)) != NULL) // Чтение файлов из заданной директории
      if (filedir->d_name[0] != '.')
         if (filedir->d_type == DT_DIR)
            printf("Directory - %s\n", filedir->d_name);
         else
         {
            filenodirs[files_count] = filedir->d_name; // Складываем все файлы в массив лля сортировки
            files_count++;
         }

   int i, j;
   for (i = 1; i < files_count; i++) // Сортируем файлы в алфавитном порядке
      for (j = 0; j < files_count - i; j++)
         if (strcmp(filenodirs[j], filenodirs[j + 1]) > 0)
         {
            str = filenodirs[j];
            filenodirs[j] = filenodirs[j + 1];
            filenodirs[j + 1] = str;
         }

   printf("\n");

   for (i = 0; i < files_count; i++)
   {
      sprintf(path, "%s/%s", argv[1], filenodirs[i]);
      printf("File - %s\n", filenodirs[i]);
      struct stat buff;
      stat(path, &buff);
      printf("Size - %ld bytes\nTime -  %sLinks - %ju\n\n", buff.st_size, ctime(&buff.st_mtime), buff.st_nlink);
   }

   closedir(d);
   return 0;
}

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
   DIR *d;
   char cwd[512];
   char path[50] = "";
   struct dirent *filedir;
   struct dirent *filenodirs[50];
   struct dirent *str;
   int a = 0;
   // struct List *filenodirs;
   if (argv[1] == NULL)
   {
      printf("Error! Directory no specified!\n");
      exit(0);
   }
   d = opendir(argv[1]);

   if (!d)
   {
      printf("Error! No such directory!\n");
      exit(0);
   }

   printf("\nStart directory - (/%s)\n", argv[1]);

   while ((filedir = readdir(d)) != NULL)
   {
      if (filedir->d_type == DT_DIR && filedir->d_name[0] != '.')
      {
         printf("%s\n", filedir->d_name);
      }
      if (filedir->d_type != DT_DIR)
      {
         filenodirs[a] = filedir;
         a++;
      }
   }
   int i, j;
   for (i = 1; i < a; i++)
      for (j = 0; j < a - i; j++)
         if (strcmp(filenodirs[j]->d_name, filenodirs[j + 1]->d_name) > 0)
         {
            str = filenodirs[j];
            filenodirs[j] = filenodirs[j + 1];
            filenodirs[j + 1] = str;
         }
   for (i = 0; i < a; i++)
   {
      strcpy(path, "");
      strcat(path, "./");
      strcat(path, argv[1]);
      strcat(path, "/");
      printf("%s ", filenodirs[i]->d_name);
      strcat(path, filenodirs[i]->d_name);
      struct stat buff;
      stat(path, &buff);
      printf("Размер - %ld bytes, Время -  %s, Кол-во ссылок - %ju\n", buff.st_size, ctime(&buff.st_mtime), buff.st_nlink);
   }
   closedir(d);
   return 0;
}

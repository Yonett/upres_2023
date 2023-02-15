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
    char input[25];
    struct dirent *filedir;
    if(argv[1] == NULL)
    {
       printf("Error! Directory no specified!\n");
       exit(0);   
    }   
    d = opendir(argv[1]);

    if(!d)
    {
       printf("Error! No such directory!\n");
       exit(0);
    }

    printf("\nStart directory - (/%s)\n", argv[1]);
    
    while((filedir = readdir(d)) != NULL)
    {   
        if(filedir->d_type == DT_DIR && filedir->d_name[0] != '.')
            {
               printf("%s\n", filedir->d_name);
            }
    }
    closedir(d);

    struct dirent *filenodir;
    if(argv[1] == NULL)
    {
       printf("Error! Directory no specified!\n");
       exit(0);
    }
    d = opendir(argv[1]);

    if(!d)
    {
       printf("Error! No such directory!\n");
       exit(0);
    }
   //  while((filenodir = readdir(d)) != NULL)
   //  {
   //      if(filenodir->d_type != DT_DIR)
   //          {
   //             printf("%s ", filenodir->d_name);
   //             struct stat buff;
   //             stat(filenodir, &buff);
   //             time(&buff.st_mtime);
   //             printf("Размер - %ld bytes, Время22 -  %s, Кол-во ссылок - %ju\n",  buff.st_size, ctime(&buff.st_mtime), buff.st_nlink);
   //          }
   //  }
    struct stat buff;
    stat("/home/NSTU/pmi-b0507/upres/lab1/test/file.txt", &buff);
    printf("Размер - %ld bytes, Время -  %s, Кол-во ссылок - %ju\n",  buff.st_size, ctime(&buff.st_mtime), buff.st_nlink);
    closedir(d);
    return 0;

}

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("need 2 args\n");
        return -1;
    }

    const char *dir = argv[1];

    struct stat statbuff;

    if ((lstat(dir, &statbuff)) == -1)
    {
        printf("lstat failed!\n");
        return -1;
    }

    if (S_ISDIR(statbuff.st_mode) == 0)
    {
        printf("must be a directory!\n");
        return -1;
    }

    DIR *pdir = opendir(dir);
    if (pdir == NULL)
    {
        printf("can not be open: %s\n", dir);
        return -1;
    }

    struct dirent *pdirentry;

    while ((pdirentry = readdir(pdir)) != NULL)
    {
        if(strcmp(pdirentry->d_name, ".") == 0
        || strcmp(pdirentry->d_name, "..") == 0){
            continue;
        }
        printf("%s\n", pdirentry->d_name);
    }

    if (closedir(pdir) == -1)
    {
        printf("can not close directory: %s\n", dir);
    }


    printf("done!\n");
    return 0;
}
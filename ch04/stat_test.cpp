#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    struct stat buf;
    const char* ptr;

    for(int i=1; i< argc; i++){
        if((lstat(argv[i], &buf) < 0)){
            printf("lstat failed: %s\n", strerror(errno));
            exit(0);
        }

        if(S_ISDIR(buf.st_mode)) ptr = "directory";
        else if(S_ISREG(buf.st_mode)) ptr = "regular file";
        else if(S_ISCHR(buf.st_mode)) ptr = "character special";
        else if(S_ISBLK(buf.st_mode)) ptr="block";
        else if(S_ISFIFO(buf.st_mode)) ptr="fifo";
        else if(S_ISSOCK(buf.st_mode)) ptr="socket";
        else if(S_ISLNK(buf.st_mode)) ptr="symbolic link";
        else ptr="unknow file type";

        printf("%s is %s\n", argv[i], ptr);
    }
    return 0;
}
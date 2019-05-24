#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>


int main(){
    const char* filename = "test.file";

/*
    if file is open with O_APPEND, 
    then the following write will all happened at
    the end of file, regardless of whatever the current
    file pointer is.

    In effect, O_APPEND only affects the behavior of write
    , but not that of read. Whatever how the current position
     of a file is changed by lseek, write will always append-only.
*/
    int fd = open(filename, O_RDWR | O_APPEND);

    if(fd== -1){
        printf("open failed: %s", strerror(errno));
        _exit(0);
    }

    int r = lseek(fd, 0, SEEK_SET);
    if(r == -1){
        printf("lseek failed: %s\n", strerror(errno));
        _exit(0);
    }

    r = write(fd, "abc", 3);
    if(r != strlen(filename)){
        printf("write failed: %s\n", strerror(errno));
        exit(0);
    }

    printf("success!\n");
    return 0;
}
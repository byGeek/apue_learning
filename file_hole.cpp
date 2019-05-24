#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

#define PRINTF_ERR(msg) printf("%s, %s\n", msg, strerror(errno));

int main(int argc, char* args[]){
    if(argc <2){
        printf("usage: file_hole filename\n ");
        exit(-1);
    }

    char* str = args[1];

/*
check if file exist?
*/
    int fd = open(str, O_RDONLY);
    if(fd != -1){
        close(fd);
        printf("file already exist, exit\n");
        exit(0);
    }

    fd = creat(str, S_IRUSR | S_IWUSR);
    if(fd == -1){
        printf("create file failed, %s\n", strerror(errno));
        exit(-1);
    }

    const char* data = "hello";
    if(lseek(fd, 1024, SEEK_SET) == -1){
        PRINTF_ERR("lseek failed");
        exit(-1);
    }

    if(write(fd, data, strlen(data)) != strlen(data)){
        PRINTF_ERR("write failed");
        exit(-1);
    }

    return 0;
}
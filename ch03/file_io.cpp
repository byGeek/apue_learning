#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

int main(int cnt, char* args[]){

    const char* filename = "test.file";
    int fd;

    if((fd = open(filename, O_RDWR | O_CREAT, S_IRUSR|S_IWUSR)) == -1){
        printf("open file failed: %s, errno: %d, %s", filename, errno, strerror(errno));
        exit(0);
    }

    auto len = strlen(filename);
    if((write(fd, filename, len)) != len){
        printf("write failed: %s", strerror(errno));
        exit(0);
    }

    if((lseek(fd, 0, SEEK_SET)) == -1){
        printf("lssek failed: %s", strerror(errno));
        exit(0);
    }

    char buff[128];

    if((read(fd, buff, len)) == -1){
        printf("read failed: %s", strerror(errno));
        exit(0);
    }

    printf("\n%s\n", buff);

    return 0;
}
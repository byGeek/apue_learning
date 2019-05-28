#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define FILE_MODE S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH

int main(){
    pid_t pid;
    const char* path="tempfifo";

    int err;

    //check fifo exist or not, fifo is a file
    /*
    fifo is a actual file. we can get the file type from lstat API CALL
        */


    int fd;
    fd = open(path, O_RDWR);
    if(fd <0){
        if (errno == ENOENT){  //fifo not exist
            err = mkfifo(path, FILE_MODE);
            if (err < 0)
            {
                printf("mkfifo failed, %s\n", strerror(errno));
                exit(1);
            }
        }else{
            printf("open pipe failed\n");
            exit(1);
        }
    }

    if((pid = fork())<0){
        printf("fork failed\n");
        exit(1);
    }else if(pid > 0){
        //parent
        const char buf[] = "helloworld";
        if(write(fd, buf, sizeof(buf)) != sizeof(buf)){
            printf("write fifo failed\n");
            close(fd);
            exit(1);
        }

        close(fd);
    }else{
        char buf[128];
        int nread = read(fd, buf, sizeof(buf));
        if(nread < 0){
            printf("read pipe failed\n");
            exit(1);
        }
        buf[nread] = 0;
        printf("child read from pipe: %s\n", buf);
        close(fd);
    }

    exit(0);
}
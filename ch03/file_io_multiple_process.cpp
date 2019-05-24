#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>




int main(){
    const char* filename = "testfile";
    int fd;

    fd = open(filename, O_RDWR| O_CREAT, S_IRUSR | S_IWUSR);

    if(fd < 0 ){
        printf("open failed\n");
        exit(1);
    }

    pid_t pid;
    if((pid = fork()) <0){
        printf("fork failed\n");
        exit(1);
    }else if(pid >0){
        close(fd);   /*will close parent's file descriptor, but open file decriptor has not been closed*/
        exit(0);  //exit parent
    }

    int err = write(fd, "helloworld", 11);
    if(err < 0){
        printf("write failed");
        exit(1);
    }

    exit(0);
}
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


/*
pipe: fd1: parent -> child
pipe: fd2: parent <- child
*/

static int fd1[2], fd2[2];

void TELL_WAIT(){
    if(pipe(fd1)< 0 || pipe(fd2) <0){
        printf("pipe failed with error\n");
        exit(1);
    }
}

void TELL_PARENT(pid_t ppid){
    if(write(fd2[1], "c", 1) !=1){
        printf("write failed, %s\n", strerror(errno));
        exit(1);
    }
}

void TELL_CHILD(pid_t pid){
    if(write(fd1[1], "p", 1) !=1){
        printf("write failed, %s\n", strerror(errno));
        exit(1);
    }
}

void WAIT_PARENT(){
    char c;
    int nread = read(fd1[0], &c, 1);
    if(nread !=1){
        printf("read failed, %s\n", strerror(errno));
        exit(1);
    }
    if(c != 'p'){
        printf("incorrect data\n");
    }
}

void WAIT_CHILD(){
    char c;
    int nread = read(fd2[0], &c, 1);
    if (nread != 1)
    {
        printf("read failed\n");
        exit(1);
    }
    if (c != 'c')
    {
        printf("incorrect data\n");
    }
}
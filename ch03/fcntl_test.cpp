#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
//#include <sys/types.h>
#include <stdlib.h>

int main(int cnt, char* args[]){
    if(cnt != 2){
        printf("Usage: fcntl_test.out file_descriptor\n");
        exit(-1);
    }

    int val = 0;

    if((val =fcntl(atoi(args[1]), F_GETFL, 0)) < 0){
        printf("fcntl error for fd %d\n", atoi(args[1]));
    }

    int accmode = val & O_ACCMODE;
    if(accmode == O_RDONLY){
        printf("read only");
    }
    else if(accmode == O_WRONLY){
        printf("write only");
    }

    else if(accmode == O_RDWR){
        printf("read and write");
    }
    else{
        printf("accmode: %d", accmode);
    }

    if(val & O_APPEND){
        printf(", append");
    }
    if(val & O_NONBLOCK){
        printf(", non block");
    }

    if(val & O_ASYNC){
        printf(", async write");
    }

    putchar('\n');

    exit(0);
}
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define RW_R_R (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int lock_wrapper(int fd, int cmd, int type, int whence, off_t offset, off_t len){
    struct flock lock;

    lock.l_type = type;
    lock.l_whence = whence;
    lock.l_start = offset;
    lock.l_len = len;

    return (fcntl(fd, cmd, &lock));
}

int test_file_lock(){


    const char* filename="testfile";

    pid_t pid;
    if((pid = fork()) <0){
        printf("fork faile\n");
        exit(1);
    }else if(pid == 0){
        int fd = open(filename, O_RDWR|O_CREAT, RW_R_R);
        if(fd < 0){
            printf("create fail failed\n");
        }
        sleep(5);

        printf("child done\n");
    }else {
        int fd = open(filename, O_RDWR|O_CREAT, RW_R_R);
        if(fd < 0){
            printf("create fail failed\n");
        }
        sleep(2);
        printf("parent done\n");
    }

    
    exit(0);
    
}


int main(){
    //create a file
    const char* filename = "temp.txt";
    FILE* pf = fopen(filename, "w+");

    if(pf == NULL){
        printf("fopen failed\n");
        exit(1);
    }

    /*
    child and parent can access same file at the same time without lock ?
    */

    //fork a process
    pid_t pid;

    int err;
    if((pid = fork()) < 0){
        printf("fork failed\n");
        exit(1);
    }else if(pid == 0){
        //child

        /*
        err = lock_wrapper(fileno(pf), F_SETLK, F_WRLCK, SEEK_SET, 0, 10);
        if(err < 0){
            printf("lock failed\n");
            exit(1);
        }
        */

        fseek(pf, 0, SEEK_SET);
        fputs("hello", pf);

        sleep(5);

    }else{
        //parent 

        /*
        err = lock_wrapper(fileno(pf), F_SETLK, F_WRLCK, SEEK_SET, 10, 10);
        if(err < 0){
            printf("lock failed\n");
            exit(1);
        }
        */
        sleep(1);
        fseek(pf, 10, SEEK_SET);

        err = fputs("world", pf);
        if(err <0){
            printf("fputs failed\n");
            exit(1);
        }
    }

    exit(0);

    //parent lock first 10 byte, write hello

    //child lock next 10 byte, write world

    //see thre result

}
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char* argv[]){
    pid_t pid;

    if((pid = fork()) <0){
        printf("fork failed with %d\n", errno);
        exit(0);
    }else if(pid >0){
        exit(0);  //exit parent process
    }else{
        //child process
        sleep(2);  //make sure parent process has died, and init process become parent process of this child process
        printf("pid=%ld, ppid=%ld, sid=%ld, pgid=%ld\n", (long)getpid(), (long)getppid(),
            (long)getsid(getpid()), (long)getpgrp());


        printf("after calling setsid to create new session\n");
        if(setsid() == -1){
            printf("setsid failed with %d\n", errno);
            exit(0);
        }

        printf("pid=%ld, ppid=%ld, sid=%ld, gid=%ld\n", (long)getpid(), (long)getppid(),
            (long)getsid(getpid()), (long)getpgrp());
        
    }
    exit(0);
}
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

extern void TELL_WAIT();
extern void TELL_PARENT(pid_t);
extern void TELL_CHILD(pid_t);
extern void WAIT_PARENT();
extern void WAIT_CHILD();


int main(){
    pid_t pid;

    TELL_WAIT();

    if((pid = fork())<0){
        printf("fork failed\n");
        exit(1);
    } else if (pid >0){
        printf("parent prepare to exe\n");
        sleep(1);
        printf("parent sleep for 1 sec\n");
        sleep(1);
        TELL_CHILD(pid);
        WAIT_CHILD();
        printf("all done");
    }else{
        printf("chilid prepare to exe\n");
        WAIT_PARENT();
        printf("now child start to exe\n");
        sleep(1);
        TELL_PARENT(getppid());
    }

    exit(0);

}
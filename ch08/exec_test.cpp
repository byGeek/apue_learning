#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

int main(void){
    pid_t pid;

    if((pid = fork()) < 0){
        printf("fock failed\n");
        exit(-1);
    }else if(pid == 0){
        if(execl("./print_args.out", "arg1", "arg2", (char*)0) < 0){
            printf("execl failed!, reason: %s\n", strerror(errno));
        }

        exit(0);
    }else{
        if(waitpid(pid, NULL, 0) < 0){
            printf("waitpid failed");
        }
        exit(0);
    }
}
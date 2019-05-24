#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

static void mysig_handle(int);

int main(int argc, char* argv[]){
    if(signal(SIGUSR1, mysig_handle) == SIG_ERR){
        printf("signal failed!\n");
        exit(0);
    }
    if(signal(SIGUSR2, mysig_handle) == SIG_ERR){
        printf("signal failed!\n");
        exit(0);
    }
    /*
    //can not catch SIGKILL and SIGSTOP signal

    if(signal(SIGKILL, mysig_handle) == SIG_ERR){
        printf("can not catch SIGKILL!\n");
        exit(0);
    }
    if(signal(SIGSTOP, mysig_handle) == SIG_ERR){
        printf("can not catch SIGSTOP!\n");
        exit(0);
    }
    */

    while (true){
        pause();  //put process to hung up state to wait for signal
    }

    
}

void mysig_handle(int sig){
    if(sig == SIGUSR1){
        printf("received SIGUSR1\n");
    } else if(sig == SIGUSR2){
        printf("received SIGUSR2\n");
    } else{
        printf("received %d\n", sig);
    }
}
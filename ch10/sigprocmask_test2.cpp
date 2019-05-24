#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

static void sig_quit_handler(int sig){
    printf("in sig quit handler!\n");
    return;
}

int main(){
    sigset_t oldset, newset, pendingset;

    if(signal(SIGQUIT, sig_quit_handler) == SIG_ERR){
        printf("can not catch SIGQUIT!\n");
        exit(0);
    }

    if(sigprocmask(0, NULL, &oldset) < 0){
        printf("fail to get blocked signal\n");
        exit(0);
    }

    sigemptyset(&newset);
    sigaddset(&newset, SIGQUIT);
    if(sigprocmask(SIG_BLOCK, &newset, NULL)){
        printf("fail to set SIGQUIT to block set\n");
        exit(0);
    }

    printf("signal a SIGQUIT to test whether block or not\n");
    sleep(10);

    sigpending(&pendingset);                                                                                                                                                                                                                                                                      
    if(sigismember(&pendingset, SIGQUIT)){
        printf("SIGQUIT is in pending state\n");
    }

    if(sigprocmask(SIG_SETMASK, &oldset, NULL) <0){
        printf("can not restore oldset!\n");
        exit(0);
    }
    printf("SIGQUIT unblocked!\n");
    sleep(10);
    printf("exit!\n");
    exit(0);

}
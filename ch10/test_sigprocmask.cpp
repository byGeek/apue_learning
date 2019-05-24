#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

static void block_sig(){
    sigset_t sig_to_block;
    sigemptyset(&sig_to_block);
    sigaddset(&sig_to_block, SIGINT);
    sigaddset(&sig_to_block, SIGALRM);

    if(sigprocmask(SIG_BLOCK, &sig_to_block, NULL)){
        printf("sigprocmask failed\n");
        return;
    }
}

int main(){
    sigset_t sig_blocked;
    int errno_save = errno;


    block_sig();

/* 
    return blocked set if last param is not null in sigprocmask
*/
    if(sigprocmask(0, NULL, &sig_blocked) <0){
        printf("sigprocmask failed\n");
        exit(0);
    }

    if(sigismember(&sig_blocked, SIGINT)){
        printf("SIGINIT blocked\n");
    }
    if(sigismember(&sig_blocked, SIGALRM)){
        printf("SIGALRM blocked\n");
    }

    errno = errno_save;

}
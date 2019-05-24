#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


static int quit_flag = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void sig_quit_handler(int sig){
    printf("in sig_quit handler\n");
}

void* thd_fn1(void* arg){
    printf("in thd_fn1\n");


    int err, sig;
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGQUIT);
    sigaddset(&set, SIGINT);

    sigset_t tset;
    sigemptyset(&tset);
    pthread_sigmask(0, NULL, &tset);
    if(sigismember(&tset, SIGQUIT)){
        printf("SIGQUIT blocked\n");
    }
    if(sigismember(&tset, SIGINT)){
        printf("SIGQUIT is blocked\n");
    }

    for(;;){
        printf("thd_fn1 wait for signal\n");  //can not receive signal ?? if MAIN thread not block signal
        err = sigwait(&set, &sig);
        if(err != 0){
            printf("sigwait failed\n");
            exit(0);  //exit the process?
        }

        switch(sig){
            case SIGQUIT:
                printf("received SIGQUIT signal\n");
                pthread_mutex_lock(&mutex);
                quit_flag = 1;
                pthread_mutex_unlock(&mutex);
                pthread_cond_signal(&cond);  //signal conditional variable
                return 0;

            case SIGINT:
                printf("received SIGINT signal\n");
                break;

            default:
                printf("received unknown signal: %d\n", sig);
                exit(1);

        }
    }
}


int main(){
    pthread_t tid;
    int err;
    /*
    if set signal handler


    struct sigaction oact, act;

    act.sa_handler = sig_quit_handler;
    act.sa_flags = 0;

    err = sigaction(SIGQUIT, &act, &oact);
    if(err != 0){
        printf("sigaction failed\n");
        exit(0);
    }
    */

    sigset_t sigs, oldmask;
    sigemptyset(&sigs);
    sigaddset(&sigs, SIGQUIT);
    sigaddset(&sigs, SIGINT);

    /*block SIGINT/SIGQUIT in main thread, so the other thread can deal with these signals*/
    err = pthread_sigmask(SIG_BLOCK, &sigs, &oldmask);
    if(err != 0){
        printf("pthread_sigmask failed\n");
        exit(1);
    }

    err = pthread_create(&tid, NULL, thd_fn1, NULL);
    if(err !=0){
        printf("pthread_create failed\n");
        exit(1);
    }

    pthread_mutex_lock(&mutex);

    printf("wait for signal\n");
    while(quit_flag == 0){
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);

    err = pthread_sigmask(SIG_SETMASK, &oldmask, NULL);
    if(err !=0){
        printf("pthread_create failed\n");
        exit(1);
    }
    printf("done\n");

    exit(0);
}
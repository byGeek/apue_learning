#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

static void my_sleep(int seconds);
static volatile bool sig_is_alarm = false;

int main(){
    my_sleep(3);

}

static void alarm_handler(int sig){
    sig_is_alarm = true;
}

void my_sleep(int seconds){
    if(signal(SIGALRM, alarm_handler) == SIG_ERR){
        printf("catch SIGALRM failed.\n");
        exit(0);
    }
    alarm(seconds);
    while(true){
        pause();
        if(sig_is_alarm){
            break;
        }
    }

    sig_is_alarm = false;
}
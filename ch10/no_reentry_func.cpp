#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pwd.h>
#include <string.h>

static void read_passwd(int);

int main(){
    if(signal(SIGALRM, read_passwd) == SIG_ERR  ){
        printf("can not catch SIGALRM!\n");
        exit(0);
    }
    /*if ignore SIGINT, then ctrl + C wont terminate the process*/
    if(signal(SIGINT, SIG_IGN) == SIG_ERR){
        printf("can not ignore SIGINT!\n");
        exit(0);
    }

    alarm(1);
    struct passwd* ptr;
    while(true){
        printf("in main while loop\n");
        if((ptr = getpwnam("robert") )== NULL){
            printf("no user name found!\n");
            break;
        }
        if(strcmp(ptr->pw_name, "robert") != 0){
            printf("return value corrupt! pw_name=%s\n", ptr->pw_name);
        }
    }
}

void read_passwd(int sig){
    struct passwd* ptr;
    printf("in read_passwd\n");

    if((ptr = getpwnam("root")) == NULL){
        printf("getpwnam error\n");
    }
    alarm(1);
}
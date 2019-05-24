#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <syslog.h>

void err_exit(const char* msg, int exitcode){
    printf("%s\n", msg);
    exit(exitcode);
}



void daemonlize(){
    
    umask(0); //clear all file flags

    pid_t pid;

    if((pid = fork()) <0){
        err_exit("fork failed", -1);
    }else if(pid > 0){
        exit(0);  //exit parent process
    }

    //child process continue
    setsid();  //create a new session

    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if(sigaction(SIGHUP, &sa, NULL) < 0)    {
        err_exit("can not catch SIGIGN", -1);
    }


    /*why fork again ?*/
    if((pid = fork()) <0){
        err_exit("can not fork", -1);
    }else if(pid > 0){
        exit(0);  //exit parent process 
    }

    if(chdir("/") < 0){
        err_exit("chdir failed", 1);
    }

    //get file describer limit and close all of it.
    struct rlimit rl;
    if(getrlimit(RLIMIT_NOFILE, &rl) < 0){
        err_exit("getrlimit failed", -1);
    }

    for(int i=0; i< rl.rlim_max; i++){
        close(i);
    }

    int fd0, fd1, fd2;
    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(fd0);
    fd2 = dup(fd0);

    openlog("test", LOG_CONS, LOG_DAEMON);
    if(fd0 != 0
    || fd1 !=1
    || fd2 !=2){
        syslog(LOG_ERR, "unexpected file descriptors %d %d %d", fd0, fd1, fd2);
        exit(1);
    }

}

int main(){
    daemonlize();
    pause();

    /*ps -ajx | grep a.out*/
    /*find new created daemon process*/

    printf("done!");
}
#include <unistd.h>
#include <stdio.h>

static int g_tmp = 1;

int main(){
    int a_tmp = 2;
    const char* buf = "hello";

    pid_t pid;
    if((pid = fork()) < 0){
        printf("fork error\n");
        _exit(0);
    } else if(pid == 0){
        //child process
        g_tmp++;
        a_tmp++;
        buf = "world";
    } else{
        sleep(2);
    }

    printf("pid: %d, g_tmp: %d, a_tmp: %d, buf: %s\n", 
            getpid(), g_tmp, a_tmp, buf);

    _exit(0);

}
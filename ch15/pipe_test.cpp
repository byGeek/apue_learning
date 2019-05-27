#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

int main(){

    pid_t pid;

    int fd[2];
    if(pipe(fd) < 0){
        printf("pipe failed\n");
        exit(1);
    }

    if((pid == fork())<0){
        assert(0);
    } else if(pid > 0){
        //parent
        close(fd[0]);
        //assert(write(fd[1], "p", 1) == 1);
        write(fd[1], "helloworld\n", 12);
        printf("parent write finished\n");
        exit(0);
    }else{
        //child
        sleep(1);
        close(fd[1]);
        char buf[128];
        int nread = read(fd[0], buf, sizeof(buf));
        if(nread < 0){
            assert(0);
        }
        //printf("read from parent: %s\n", buf);
        write(STDOUT_FILENO, buf, nread);
        exit(0);
    }


}
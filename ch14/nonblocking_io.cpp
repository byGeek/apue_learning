#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(){
    char buf[102400];
    int cnt;

    cnt = read(STDIN_FILENO, buf, sizeof(buf));
    if(cnt <0){
        fprintf(stderr, "read failed\n");
        exit(1);
    }

    fprintf(stderr, "read %d bytes\n", cnt);

    int flags = fcntl(STDOUT_FILENO, F_GETFL, 0);
    if(fcntl(STDOUT_FILENO, F_SETFL, flags | O_NONBLOCK) < 0){
        fprintf(stderr, "fcntl failed\n");
        exit(1);
    }

    /*

    set STDOUT_FILENO nonblocking, write system call will return EAGAIN
    if last write is not complete.
    */


    int remain = cnt;
    char* ptr = buf;
    while(remain > 0){
        errno = 0;
        cnt = write(STDOUT_FILENO, ptr, remain);
        fprintf(stderr, "cnt=%d, errno=%d", cnt, errno);
        if(cnt > 0){
            remain -= cnt;
            ptr += cnt;
        } else {
            fprintf(stderr, "write failed\n");
            exit(1);
        }

    }

    exit(0);

}
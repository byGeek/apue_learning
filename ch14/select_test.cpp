#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <assert.h>
#include <errno.h>
#include <string.h>

#define FILE_MODE S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH

int set_nonblocking(int fd){
    int flags;
    flags= fcntl(fd, F_GETFL, 0);
    if(flags < 0){
        assert(0);
    }
    flags |= O_NONBLOCK;
    return fcntl(fd, F_SETFL, flags);
}

int main(){


    //copy file, set io non-blocking

    int fdi, fdo;
    const char* input_file = "testfile";
    const char* output_file = "out_file";

    fdi = open(input_file, O_RDONLY);
    if(fdi < 0){
        printf("open failed: %s\n", strerror(errno));
        assert(0);
    }

    fdo = open(output_file, O_RDWR |O_CREAT|O_TRUNC, FILE_MODE);
    if(fdo < 0){
        assert(0);
    }

    //set fd non-blocking
    // set_nonblocking(fdi);
    // set_nonblocking(fdo);

    fd_set readset, writeset;
    

    int nfds;
    if(fdi > fdo){
        nfds = fdi +1;
    }else{
        nfds = fdo +1;
    }

    char buf[4096];

    //init read
    int nread, nwrite;
    // nread = read(fdi, buf, sizeof(buf));
    // if(nread < 0 && errno != EAGAIN){
    //     assert(0);
    // }
    while(true){
        FD_ZERO(&readset);
        FD_ZERO(&writeset);
        FD_SET(fdi, &readset);
        FD_SET(fdo, &writeset);

        int err = select(nfds, &readset, &writeset, NULL, NULL);
        if(err == -1){
            assert(0);
        }
        //readable
        if(FD_ISSET(fdi, &readset)){
            // nwrite = write(fdo, buf, nread);
            // if(nwrite < 0 && errno != EAGAIN){
            //     assert(0);
            // }

            nread = read(fdi, buf, sizeof(buf));
            if(nread < 0){
                assert(0);
            }else if(nread == 0){
                printf("read eof!\n");
                break;
            }
        }

        //writable
        if(FD_ISSET(fdo, &writeset)){
            nwrite = write(fdo, buf, nread);
            assert(nwrite == nread);
        }
    }

}
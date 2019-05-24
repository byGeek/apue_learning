#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char* argv[]){
    if(argc !=2){
        exit(0);
    }

    char* ttt = "123";

    const char* filename = argv[1];

    FILE* pfile;
    if((pfile =fopen(filename, "r")) == NULL){
        printf("fopen failed!\n");
        exit(0);
    }

    int fd = fileno(pfile);

    struct stat buf;
    if(fstat(fd, &buf) < 0){
        printf("fstat failed\n");
        exit(-1);
    }

    //char* ret = buf
}